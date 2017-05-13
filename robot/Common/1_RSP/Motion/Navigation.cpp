#include <Arduino.h>
#include "core/ArdFramework.h"
#include "K_constants.h"
#include "Log.h"
#include "Navigation.h"
#include "Actuators/Buzzer.h"
#include "OppDetection.h"
#include "RobotParameters.h"
#include "Graph.h"

using namespace ard;

/* Angle delta under which we consider target is reached 2.5mm on 5m */
#define NO_TURN_DELTA 0.0005 //rad
#define NO_MOVE_DELTA 1      //mm
#define KLAXON_FREQ 1000     //Hz
#define RECAL_FORCING 400     //mm
#define RECAL_ESCAPE_MARGIN 30 //mm
#define RECAL_TIMEOUT 5000 //ms
#define GRAPH_TIMEOUT 15000 //ms
#define OPP_IMPATIENCE_TIMEOUT 5000 //ms
#define CHECK_ONE_ORDER_AT_A_TIME() ASSERT_TEXT((m_state == eNavState_IDLE || m_state == eNavState_BLOCKED) && m_order == eNavOrder_NOTHING, "Nav cannot do 2 orders at a time");

Navigation::Navigation(Buzzer& klaxon, OppDetection& detection, Graph& graph)
        :
                Thread("Nav", PRIO_NAVIGATION, STACK_NAVIGATION, PERIOD_NAVIGATION),
                switchRecalFL(BORDURE_AVG, 1000, 10),
                switchRecalFR(BORDURE_AVD, 1000, 10),
                switchRecalRC(BORDURE_ARC, 1000, 10),
                m_pose(),
                m_state(eNavState_IDLE),
                m_target(),
                m_targetDir(eDir_BEST),
                m_order(eNavOrder_NOTHING),
                userMaxSpeed(0),
                userMaxTurnSpeed(0),
                userMaxAcc(0),
                userMaxTurnAcc(0),
                stepperL(AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR),
                stepperR(AccelStepper::DRIVER, PAPD_STEP, PAPD_DIR),
                m_color(eColor_PREF),
                m_mutex(),
                oldStepL(0),
                oldStepR(0),
                currentWayPoint(0),
                m_graphDir(eDir_BEST),
                conf(NULL),
                noSwitchMode(false),
                state(),
                klaxon(klaxon),
                detection(detection),
                graph(graph)
{
    state = apb_NavState_init_default;
}

void Navigation::updateConf(RobotParameters* newConf)
{
    ASSERT(newConf);
    conf = newConf;

    userMaxSpeed        = conf->maxSpeed();
    userMaxTurnSpeed    = conf->maxTurnSpeed();
    userMaxAcc          = conf->maxAcc();
    userMaxTurnAcc      = conf->maxTurnAcc();
}

/**---------------------------------
 * Thread interface
 ---------------------------------*/

void Navigation::init()
{
    ASSERT_TEXT(conf, "You must bind a configuration.");
    Thread::init();
}

void Navigation::run()
{
    compute_odom();

    //Take a mutex to prevent localisation and target to be changed during a cycle
    m_mutex.lock();

    //General transition
    if(orderTimeout.isFired())
    {
        orderTimeout.cancel();
        LOG_ERROR("Order timeout, interrupt order=" + orderToString(m_order) + " in state=" + stateToString(m_state) + " and go to error.");
        m_mutex.unlock();
        stopMoving(eNavState_STOPPING_IN_BLOCKED);
        m_mutex.lock();
    }

    switch (m_state)
    {
        default:
            ASSERT_TEXT(false,"Nav : Default state in switch case.");
            break;

        case eNavState_IDLE:
        {
            orderTimeout.cancel();
            break;
        }

        case eNavState_BLOCKED:
        {
            break;
        }

/** -----------------------------------------------------------------------------------
 * GOTO
 --------------------------------------------------------------------------------------*/
        case eNavState_FACING_DEST:
        {
            if (subOrderFinished())
            {
                //Request straight line
                action_goingToTarget();
            }
            break;
        }

        case eNavState_GOING_TO_TARGET:
        {
            //This is the MOST important thing to have a working robot
            if(m_targetDir == eDir_BACKWARD)
                klaxon.naderBell();

            if (subOrderFinished())
            {
                //Request rotation to final heading
                action_turningAtTarget();
            }
            else
            {
                //check for opponent presence (if avoidance system is active)
                if( detection.isOpponentOnPath(m_targetDir, m_pose) )
                {
                    action_waitOppMove();
                }
            }

            break;
        }

        case eNavState_TURNING_AT_TARGET:
        {
            if (subOrderFinished())
            {
                //Change state to terminate order
                action_finishOrder();
            }
            break;
        }


/** -----------------------------------------------------------------------------------
 * Order interruption
 --------------------------------------------------------------------------------------*/
        case eNavState_STOPPING:
        {
            if (subOrderFinished())
            {
                //Change state to terminate order
                m_state = eNavState_IDLE;
                m_order = eNavOrder_NOTHING;
                LOG_INFO("stopped.");
            }
            break;
        }

        case eNavState_STOPPING_IN_BLOCKED:
        {
            if (subOrderFinished())
            {
                //Change state to terminate order
                m_state = eNavState_BLOCKED;
                m_order = eNavOrder_NOTHING;
                LOG_INFO("stopped.");
            }
            break;
        }

/** -----------------------------------------------------------------------------------
 * Avoidance
 --------------------------------------------------------------------------------------*/

        case eNavState_WAIT_OPP_MOVE:
        {
            //Check if opponent has left
            if( !detection.isOpponentOnPath(m_targetDir, m_pose) )
            {
                LOG_INFO("Opponent has moved away, continuing order.");
                action_goingToTarget();
            }
            //If opponent is still present, continue to wait
            //else
            //    LOG_DEBUG("Opponent is still present, continuing to wait.");
            break;
        }

/** -----------------------------------------------------------------------------------
 * Recal on Border
 --------------------------------------------------------------------------------------*/

        case eNavState_FACING_WALL:
        {
            if (subOrderFinished())
            {
                switch (m_order) {
                    case eNavOrder_RECAL_FACE:
                        //Request straight line
                        applyCmdToGoStraight(RECAL_FORCING, conf->recalSpeed(), userMaxAcc);
                        break;
                    case eNavOrder_RECAL_REAR:
                        //Request straight line
                        applyCmdToGoStraight(-RECAL_FORCING, conf->recalSpeed(), userMaxAcc);
                        break;
                    default:
                        ASSERT(false); //should not be reached, dev bug
                        break;
                }

                m_state = eNavState_CONTACTING_WALL;
            }
            break;
        }

        case eNavState_CONTACTING_WALL:
        {
            //Most important thing EVER
            klaxon.naderBell();

            //If target is reached (in switch mode) then the recal failed
            if(!noSwitchMode && subOrderFinished())
            {
                LOG_ERROR("Failed to recal on border.");
                klaxon.bip(5);
                m_order = eNavOrder_NOTHING;
                m_state = eNavState_IDLE;
            }
            //If both switch are contacted (or target reached in no switch mode)
            else if( (noSwitchMode  && subOrderFinished())
                  || (!noSwitchMode && m_order == eNavOrder_RECAL_FACE && switchRecalFL.read() && switchRecalFR.read())
                  || (!noSwitchMode && m_order == eNavOrder_RECAL_REAR && switchRecalRC.read()))
            {
                LOG_INFO(String("   wall touched"));

                enterCriticalSection();
                stepperL.setCurrentPosition(0);
                oldStepL = 0;
                stepperL.move(0); //move(0) is equivalent to stay on current position
                stepperR.setCurrentPosition(0);
                oldStepR = 0;
                stepperR.move(0);
                setPosition(m_target, false);//position has already been symetrized
                exitCriticalSection();

                //Escape from wall
                double distance = 0;
                if(m_order == eNavOrder_RECAL_FACE)
                {
                    distance = conf->xouter() - conf->xav() + RECAL_ESCAPE_MARGIN;
                    m_targetDir = eDir_BACKWARD;
                }
                else if(m_order == eNavOrder_RECAL_REAR)
                {
                    distance = conf->xouter() - conf->xar() + RECAL_ESCAPE_MARGIN;
                    m_targetDir = eDir_FORWARD;
                }
                else
                    ASSERT(false);
                m_target.translatePolar(m_target.hDegree(), m_targetDir * distance);
                applyCmdToGoStraight(m_targetDir * distance, userMaxSpeed, userMaxAcc);
                m_state = eNavState_ESCAPING_WALL;
                orderTimeout.arm(1000+OPP_IMPATIENCE_TIMEOUT);
                LOG_INFO(String("   escaping of distance=") + distance + " to reach pose=" + m_target.toString()+".");
                klaxon.bip(1);
            }
            break;
        }

        case eNavState_ESCAPING_WALL:
        {
            //check for opponent presence (if avoidance system is active)
            if( (m_order == eNavOrder_RECAL_REAR && detection.isOpponentAhead(m_pose))
            || (m_order == eNavOrder_RECAL_FACE && detection.isOpponentBehind(m_pose)) )
            {
                //Stops as an opponent is detected
                enterCriticalSection();
                stepperL.stop();
                stepperR.stop();
                exitCriticalSection();

                m_state = eNavState_WAIT_OPP_ESCAPE_RECALL;
                LOG_INFO("       Opponent is blocking recal escape");
            }
            else if(subOrderFinished())
            {
                LOG_INFO(String("   recal finished, exit position : ")+ m_pose.toString());
                m_order = eNavOrder_NOTHING;
                m_state = eNavState_IDLE;
            }
            break;
        }

        //This avoidance case is particular as the robot is stucked against the wall, so it will try to move as soon as possible
        case eNavState_WAIT_OPP_ESCAPE_RECALL:
        {
            //check for opponent presence (if avoidance system is active)
            if( (m_order == eNavOrder_RECAL_REAR && detection.isOpponentAhead(m_pose))
            || (m_order == eNavOrder_RECAL_FACE && detection.isOpponentBehind(m_pose)) )
            {
                klaxon.playTone(KLAXON_FREQ, PERIOD_NAVIGATION);
            }
            else
            {
                m_state = eNavState_ESCAPING_WALL;
                LOG_INFO("       Opponent moved away");

                //Reset order
                ASSERT(m_targetDir);
                applyCmdToGoStraight(m_targetDir * m_pose.distanceTo(m_target), userMaxSpeed, userMaxAcc);
            }
            break;
        }

        /** -----------------------------------------------------------------------------------
         * GRAPH MANAGEMENT
         --------------------------------------------------------------------------------------*/
        case eNavState_COMPUTING_GRAPH:
        {
            if(!graph.computeShortertPath(m_pose.toAmbiPose(m_color), m_target.toAmbiPose(m_color), m_graphDir))
            {
                LOG_ERROR("No path found in graph !");
                m_order = eNavOrder_NOTHING;
                m_state = eNavState_BLOCKED;
                return;
            }
            else
            {
                //Go to first point
                currentWayPoint = 0;
                action_gotoNextWaypoint();
            }
            break;
        }

        /** -----------------------------------------------------------------------------------
         *  --------------------------------
         --------------------------------------------------------------------------------------*/

    }
    m_mutex.unlock();

    static auto lastState = m_state;
    if (m_state != lastState)
    {
        LOG_DEBUG("state changed from  " + stateToString(lastState) + " to " + stateToString(m_state));
        lastState = m_state;
    }
}

void Navigation::updateFromInterrupt()
{
    stepperL.run();
    stepperR.run();
}

/**---------------------------------
 * User (= strategy) interface
 ---------------------------------*/

void Navigation::setPosition(PointCap newPose, bool sym)
{
    //prevent any interrupt from occurring so that position fields are not corrupted
    enterCriticalSection();
    if(sym)
        m_pose = newPose.toAmbiPose(m_color);
    else
        m_pose = newPose;
    exitCriticalSection();

    LOG_INFO("   position set to :" + newPose.toString());
}


void ard::Navigation::setSpeedAcc(uint16_t vMax, uint16_t vMaxTurn, uint16_t accMax, uint16_t accMaxTurn)
{
    m_mutex.lock();

    CHECK_ONE_ORDER_AT_A_TIME();

    //If a config is NULL, then restore default configuration
    if( vMax != 0 )
    {
        LOG_ERROR("New vMax config saturated");
        userMaxSpeed = saturate(vMax, RobotParameters::MINBOUND_MAXSPEED, RobotParameters::MAXBOUND_MAXSPEED);
    }
    else
        userMaxSpeed = conf->maxSpeed();

    //If a config is NULL, then restore default configuration
    if( vMaxTurn    != 0 )
    {
        LOG_ERROR("New vMaxTurn config saturated");
        userMaxTurnSpeed = saturate(vMaxTurn, RobotParameters::MINBOUND_MAXTURNSPEED, RobotParameters::MAXBOUND_MAXTURNSPEED);
    }
    else
        userMaxTurnSpeed = conf->maxTurnSpeed();

    //If a config is NULL, then restore default configuration
    if( accMax != 0 )
    {
        LOG_ERROR("New accMax config saturated");
        userMaxAcc = saturate(accMax, RobotParameters::MINBOUND_MAXACC, RobotParameters::MAXBOUND_MAXACC);
    }
    else
        userMaxAcc = conf->maxAcc();

    //If a config is NULL, then restore default configuration
    if( accMaxTurn != 0 )
    {
        LOG_ERROR("New accMaxTurn config saturated");
        userMaxTurnAcc = saturate(accMaxTurn, RobotParameters::MINBOUND_MAXTURNACC, RobotParameters::MAXBOUND_MAXTURNACC);
    }
    else
        userMaxTurnAcc = conf->maxTurnAcc();

    m_mutex.unlock();

    LOG_INFO(String("    speed set to : (") + userMaxSpeed + "mm/s, " + userMaxTurnSpeed +
            "°/s), acc set to (" + userMaxAcc + "mm/s², " + userMaxTurnAcc + "°/s²)");
}

void Navigation::goTo(Point target, eDir sens, bool sym)
{
    m_mutex.lock();
    CHECK_ONE_ORDER_AT_A_TIME();

    m_order = eNavOrder_GOTO;

    //Symetrize target
    if(sym)
        m_target = target.toAmbiPoint(m_color);
    else
        m_target = target;

    //Choose direction
    DelayMs moveDur = 0;
    if(sens == eDir_BEST)
    {
        m_targetDir = findOptimalDir(m_pose, m_target, false, &moveDur);
    }
    else
    {
        moveDur = motionDuration(m_pose, m_target, sens, false);
        m_targetDir = sens;
    }

    LOG_INFO("   new request : goTo" + m_target.toString() + " "  + sensToString(sens) + ".");
    action_startOrder();

    orderTimeout.arm(moveDur + OPP_IMPATIENCE_TIMEOUT);

    m_mutex.unlock();
}

void Navigation::goToCap(PointCap target, eDir sens, bool sym)
{
    m_mutex.lock();
    CHECK_ONE_ORDER_AT_A_TIME();

    m_order = eNavOrder_GOTO_CAP;

    //Symetrize target
    if(sym)
        m_target = target.toAmbiPose(m_color);
    else
        m_target = target;

    //Choose direction
    DelayMs moveDur = 0;
    if(sens == eDir_BEST)
    {
        m_targetDir = findOptimalDir(m_pose, m_target, true, &moveDur);
    }
    else
    {
        moveDur = motionDuration(m_pose, m_target, sens, true);
        m_targetDir = sens;
    }

    LOG_INFO("   new request : goToCap" + m_target.toString() + " "  + sensToString(sens) + ".");
    action_startOrder();

    orderTimeout.arm(moveDur + OPP_IMPATIENCE_TIMEOUT);

    m_mutex.unlock();
}

void Navigation::goForward(float distanceMm)
{
    LOG_INFO(String("   new request : goForward(") + distanceMm + "mm).");

    m_mutex.lock();
    PointCap target = m_pose;
    m_mutex.unlock();

    target.translatePolar(m_pose.hDegree(), distanceMm);

    if(0 <= distanceMm)
    {
        goTo(target, eDir_FORWARD, false);
    }
    else
    {
        goTo(target, eDir_BACKWARD, false);
    }
}

void Navigation::turnDelta(float angle, bool sym)
{
    LOG_INFO(String("   new request : turnDelta(") + angle + "°).");

    m_mutex.lock();
    PointCap target = m_pose;
    m_mutex.unlock();
    if(sym && m_color == eColor_SYM)
        target.hDegree(target.hDegree() - angle);
    else
        target.hDegree(target.hDegree() + angle);

    goToCap(target, eDir_FORWARD, false);
}

void Navigation::turnTo(float angle, bool sym)
{
    LOG_INFO(String("   new request : turnTo(") + angle + "°).");

    m_mutex.lock();
    PointCap target = m_pose;
    m_mutex.unlock();
    if(sym && m_color == eColor_SYM)
        target.hDegree(-angle);
    else
        target.hDegree(angle);

    goToCap(target, eDir_FORWARD, false);
}

void Navigation::faceTo(Point p, bool sym)
{
    LOG_INFO(String("   new request : faceTo") + p.toString() + ".");

    m_mutex.lock();
    PointCap target = m_pose;
    m_mutex.unlock();
    if( sym )
        target.h = m_pose.angleTo(p.toAmbiPoint(m_color));
    else
        target.h = m_pose.angleTo(p);

    goToCap(target, eDir_FORWARD, true);
}

void Navigation::rearTo(Point p, bool sym)
{
    LOG_INFO(String("   new request : rearTo") + p.toString() + ".");

    m_mutex.lock();
    PointCap target = m_pose;
    m_mutex.unlock();
    if( sym )
        target.h = headingToDir(m_pose.angleTo(p.toAmbiPoint(m_color)), eDir_BACKWARD);
    else
        target.h =headingToDir(m_pose.angleTo(p), eDir_BACKWARD);

    goToCap(target, eDir_BACKWARD, true);
}

void Navigation::recalFace(eTableBorder border)
{
    LOG_INFO(String("   new request : recalFace on border=") + border + ".");

    m_mutex.lock();
    CHECK_ONE_ORDER_AT_A_TIME()
    m_order = eNavOrder_RECAL_FACE;
    m_targetDir = eDir_FORWARD;
    m_target = getRecalPointFace(border).toAmbiPose(m_color);
    double angleDelta = moduloPiPi(m_target.h - m_pose.h);

    //In case the switch are not mapped, they will always be set to 1, so we deactivate the switch reading
    if( switchRecalFL.read() && switchRecalFR.read() )
        noSwitchMode = true;
    else
        noSwitchMode = false;

    //Request turn
    LOG_INFO(String("    Facing wall to recal at ") + m_target.toString() + "...");
    applyCmdToTurn(angleDelta, userMaxSpeed, userMaxAcc); //We will go to reset position, so let do it quickly ^^
    m_state = eNavState_FACING_WALL;

    orderTimeout.arm(RECAL_TIMEOUT);

    m_mutex.unlock();
}

void Navigation::recalRear(eTableBorder border)
{
    LOG_INFO(String("   new request : recalRear on border=") + border + ".");

    m_mutex.lock();
    CHECK_ONE_ORDER_AT_A_TIME()
    m_order = eNavOrder_RECAL_REAR;
    m_targetDir = eDir_BACKWARD;
    m_target = getRecalPointRear(border).toAmbiPose(m_color);;
    double angleDelta = moduloPiPi(m_target.h - m_pose.h);

    //In case the switch are not mapped, they will always be set to 1, so we deactivate the switch reading
    if( switchRecalRC.read() )
        noSwitchMode = true;
    else
        noSwitchMode = false;

    //Request turn
    LOG_INFO(String("    Facing table to recal at ") + m_target.toString() + "...");
    applyCmdToTurn(angleDelta, userMaxSpeed, userMaxAcc); //We will go to reset position, so let do it quickly ^^
    m_state = eNavState_FACING_WALL;

    orderTimeout.arm(RECAL_TIMEOUT);

    m_mutex.unlock();
}

void Navigation::graphTo(PointCap target, eDir sens)
{
    m_mutex.lock();
    CHECK_ONE_ORDER_AT_A_TIME();

    LOG_INFO(String("   new request :  graphTo ") + target.toString() + ", computation in progress...");

    //Computation is delayed for a further time to prevent the caller from making the graph search
    m_target = target;
    m_state = eNavState_COMPUTING_GRAPH;
    m_targetDir = sens;
    m_graphDir = sens;
    m_order = eNavOrder_GRAPH_TO;
    orderTimeout.arm(GRAPH_TIMEOUT);

    m_mutex.unlock();
}

void Navigation::stopMoving(eNavState targetState)
{
    LOG_INFO("stop requested");
    m_mutex.lock();

    //prevent any interrupt from occurring between any configuration of a left/right motor
    enterCriticalSection();
    stepperL.stop();
    stepperR.stop();
    exitCriticalSection();

    //The state is directly changed to interrupting
    m_state = targetState;
    m_order = eNavOrder_NOTHING;

    m_mutex.unlock();
}

void Navigation::wait()
{
    //obviously don't put a mutex, it's a blocking call ... !
    while(!targetReached())
    {
        ArdOs::sleepMs(20);
    }
}

bool Navigation::targetReached()
{
    m_mutex.lock();
    bool res = m_state == eNavState_IDLE && m_order == eNavOrder_NOTHING;
    m_mutex.unlock();
    return res;
}

bool Navigation::blocked()
{
    m_mutex.lock();
    bool res = m_state == eNavState_BLOCKED && m_order == eNavOrder_NOTHING;
    m_mutex.unlock();
    return res;
}

/**---------------------------------
 * Nav configuration
 ---------------------------------*/

void Navigation::setColor(eColor c)
{
    ASSERT_TEXT(c != eColor_UNKNOWN, "color should not be set to undefined.");
    m_color = c;
}

DelayMs Navigation::motionDuration(PointCap const& start, PointCap const& end, eDir sens, bool isGotoCap)
{
    double startAngle = 0;
    double endAngle = 0;

    //Determine start and stop angles
    startAngle = degrees(fabs(headingToDir(start.angleHeadingTo(end), sens)));
    if(isGotoCap)
        endAngle = degrees(fabs(headingToDir(start.angleTo(end) - end.h, sens)));

    //compute durations
    DelayMs translationDuration = start.distanceTo(end)*1000./conf->maxSpeed();
    DelayMs rotationStartDuration = startAngle*1000. / (double)conf->maxTurnSpeed();
    DelayMs rotationEndDuration = endAngle*1000. / (double)conf->maxTurnSpeed();
    DelayMs duration = translationDuration + rotationStartDuration + rotationEndDuration;

    LOG_DEBUG(String("Order duration is : ") + duration + "ms (T="+ translationDuration + " Rs=" + rotationStartDuration + " Re=" + rotationEndDuration+")");

    return duration;
}

eDir Navigation::findOptimalDir(PointCap const& start, PointCap const& end, bool isGotoCap, DelayMs* duration)
{
    DelayMs moveDurFront = motionDuration(start, end, eDir_FORWARD, isGotoCap);
    DelayMs moveDurRear = motionDuration(start, end, eDir_BACKWARD, isGotoCap);
    if( moveDurFront < moveDurRear)
    {
        if(duration)
            *duration = moveDurFront;
        return eDir_FORWARD;
    }
    else
    {
        if(duration)
            *duration = moveDurRear;
        return eDir_BACKWARD;
    }
}

/**---------------------------------
 * Publish state
 ---------------------------------*/
apb_NavState const& Navigation::serealize()
{
    //DO NOT use mutex here, for some resons RemoteControl creates a deadlock
    state.state = m_state;
    state.order = m_order;
    state.pos = m_pose.getProto();
    state.omronFront = detection.omronFront.readRaw();
    state.omronRear = detection.omronRear.readRaw();

    state.switchRecalFL = switchRecalFL.readRaw();
    state.switchRecalFR = switchRecalFR.readRaw();
    state.switchRecalRC = switchRecalRC.readRaw();
    return state;
}

/**---------------------------------
 * Private functions
 ---------------------------------*/

void Navigation::compute_odom()
{
    //prevent any interrupt from occurring between any configuration of a left/right motor
    enterCriticalSection();
    long newStepL = stepperL.currentPosition();
    long newStepR = stepperR.currentPosition();
    exitCriticalSection();

    long dxL = newStepL - oldStepL;
    long dxR = newStepR - oldStepR;
    oldStepL = newStepL;
    oldStepR = newStepR;
    double ds = (dxR * conf->GAIN_STEPS_2_MM_RIGHT + dxL * conf->GAIN_STEPS_2_MM_LEFT) / 2.;
    double dh = (dxR * conf->GAIN_STEPS_2_MM_RIGHT - dxL * conf->GAIN_STEPS_2_MM_LEFT) / conf->voie();

    //prevent conflicts with m_pose usage
    m_mutex.lock();

    //a real cosinus/sinus is too heavy to compute, use fast maths instead
    m_pose.x += ds * cos(m_pose.h + dh);
    m_pose.y += ds * sin(m_pose.h + dh);
    m_pose.h = moduloPiPi(m_pose.h + dh);

    m_mutex.unlock();
}


void Navigation::action_startOrder()
{
    LOG_DEBUG("   new order " + orderToString(m_order) + "(" + m_target.x + ", " + m_target.y + ", " + m_target.h + ") " + sensToString(m_targetDir) + ".");
    ASSERT(m_targetDir);
    double distDelta = m_targetDir * m_pose.distanceTo(m_target);
    //Do no move if already on target
    if (fabs(distDelta) <= NO_MOVE_DELTA)
    {
        action_turningAtTarget();
    }
    else
    {
        double angleDelta = headingToDir(m_pose.angleHeadingTo(m_target), m_targetDir);

        //Do not turn if already facing right direction
        if (fabs(angleDelta) <= NO_TURN_DELTA)
        {
            //Request straight line
            action_goingToTarget();
        }
        else
        {
            //Request turn
            applyCmdToTurn(angleDelta, userMaxTurnSpeed, userMaxTurnAcc);
            //Change state
            m_state = eNavState_FACING_DEST;
        }
    }
}

void Navigation::action_goingToTarget()
{
    //Request straight line
    ASSERT(m_targetDir);
    applyCmdToGoStraight(m_targetDir * m_pose.distanceTo(m_target), userMaxSpeed, userMaxAcc);
    //Change state
    m_state = eNavState_GOING_TO_TARGET;
}

void Navigation::action_turningAtTarget()
{
    //Request rotation to final heading
    if( m_order == eNavOrder_GOTO_CAP || m_order == eNavOrder_GRAPH_TO )
    {
        double angleDelta = moduloPiPi(m_target.h - m_pose.h);
        //Do not turn if already facing right direction
        if (fabs(angleDelta) <= NO_TURN_DELTA)
        {
            //Change state to terminate order
            action_finishOrder();
        }
        else
        {
            //Request turn
            applyCmdToTurn(angleDelta, userMaxTurnSpeed, userMaxTurnAcc);
            LOG_DEBUG("target destination reached, facing end move heading.");
            //Change state
            m_state = eNavState_TURNING_AT_TARGET;
        }
    }
    else
    {
        //Change state to terminate order
        action_finishOrder();
    }
}

void Navigation::action_gotoNextWaypoint()
{
    ASSERT(1 < graph.getWayPointNb());
    ASSERT(currentWayPoint < graph.getWayPointNb());

    currentWayPoint++;
    m_target = graph.getWayPoint(currentWayPoint).toAmbiPose(m_color);
    if(m_graphDir == eDir_BEST)
        m_targetDir = findOptimalDir(m_pose, m_target, eNavOrder_GOTO_CAP);
    else
        m_targetDir = m_graphDir;
    LOG_INFO(String("   going to next waypoint ") + m_target.toString() + " "  + sensToString(m_targetDir) + ".");
    action_startOrder();
}

void Navigation::action_finishOrder()
{
    switch(m_order)
    {
        default:
        case eNavOrder_NOTHING:
            ASSERT(false);
            break;


        case eNavOrder_GOTO:
            //TODO a enlever
            //As move is expected to be perfect, correct any numerical instability by forcing position to be exactly the target
            enterCriticalSection();
            m_pose.x = m_target.x;
            m_pose.y = m_target.y;
            exitCriticalSection();
            break;


        case eNavOrder_GOTO_CAP:
            //TODO a enlever
            //As move is expected to be perfect, correct any numerical instability by forcing position to be exactly the target
            enterCriticalSection();
            m_pose.x = m_target.x;
            m_pose.y = m_target.y;
            m_pose.h = m_target.h;
            exitCriticalSection();
            break;

        case eNavOrder_GRAPH_TO:
            //If last point is not reached, continue to execute path
            if(currentWayPoint < graph.getWayPointNb()-1)
            {
                action_gotoNextWaypoint();
                return;
            }
            else
            {
                LOG_INFO(String("   graph target reached"));
                //graph.reset();
            }
            break;
    }

    
    m_state = eNavState_IDLE;
    m_order = eNavOrder_NOTHING;
    LOG_INFO("   order finished.");
}

void Navigation::action_waitOppMove()
{
    enterCriticalSection();
    stepperL.stop();
    stepperR.stop();
    exitCriticalSection();

    m_state = eNavState_WAIT_OPP_MOVE;

    //Inform user
    LOG_INFO("Waiting that opponent moves away... (blocked at position = " + m_pose.toString()+")");
    klaxon.playTone(KLAXON_FREQ, conf->detectionWaitForOppMove());
}

void Navigation::applyCmdToGoStraight(double mm, double maxSpeed, double maxAcc)
{
    LOG_DEBUG(String("applyCmdToGoStraight : ") + mm + " mm");

    double maxAccLeft = fabs(maxAcc * conf->GAIN_MM_2_STEPS_LEFT);
    double maxAccRight = fabs(maxAcc * conf->GAIN_MM_2_STEPS_RIGHT);
    double maxSpeedLeft = fabs(maxSpeed * conf->GAIN_MM_2_STEPS_LEFT);
    double maxSpeedRight = fabs(maxSpeed * conf->GAIN_MM_2_STEPS_RIGHT);
    double distLeft = mm * conf->GAIN_MM_2_STEPS_LEFT;
    double distRight = mm * conf->GAIN_MM_2_STEPS_RIGHT;

    //prevent any interrupt from occurring between any configuration of a left/right motor
    enterCriticalSection();
    stepperL.setAcceleration(maxAccLeft);
    stepperR.setAcceleration(maxAccRight);
    stepperL.setMaxSpeed(maxSpeedLeft);
    stepperR.setMaxSpeed(maxSpeedRight);
    stepperL.move(distLeft);
    stepperR.move(distRight);
    exitCriticalSection();
}

void Navigation::applyCmdToTurn(double angleInRad, double maxSpeed, double maxAcc)
{
    LOG_DEBUG(String("applyCmdToTurn : ") + angleInRad + " rad");

    double maxAccLeft = fabs(maxAcc * conf->GAIN_MM_2_STEPS_LEFT);
    double maxAccRight = fabs(maxAcc * conf->GAIN_MM_2_STEPS_RIGHT);
    double maxSpeedLeft = fabs(maxSpeed * DEG_TO_RAD * conf->GAIN_RAD_2_STEPS_LEFT);
    double maxSpeedRight = fabs(maxSpeed * DEG_TO_RAD * conf->GAIN_RAD_2_STEPS_RIGHT);
    double distLeft = -angleInRad * conf->GAIN_RAD_2_STEPS_LEFT;
    double distRight = angleInRad * conf->GAIN_RAD_2_STEPS_RIGHT;

    //prevent any interrupt from occurring between any configuration of a left/right motor
    enterCriticalSection();
    stepperL.setAcceleration(maxAccLeft);
    stepperR.setAcceleration(maxAccRight);
    stepperL.setMaxSpeed(maxSpeedLeft);
    stepperR.setMaxSpeed(maxSpeedRight);
    stepperL.move(distLeft);
    stepperR.move(distRight);
    exitCriticalSection();
}

void Navigation::interruptCurrentMove()
{
    LOG_INFO("current order is interrupted.");
    m_state = eNavState_STOPPING;
    //prevent any interrupt from occurring between any configuration of a left/right motor
    enterCriticalSection();
    stepperL.stop();
    stepperR.stop();
    exitCriticalSection();
}

/**
 * return true if the trajectory is finished
 */
bool Navigation::subOrderFinished()
{
    //prevent any interrupt from occurring between any configuration of a left/right motor
    enterCriticalSection();
    bool res = stepperL.distanceToGo() == 0 || stepperR.distanceToGo() == 0;
    exitCriticalSection();
    return res;
}

PointCap Navigation::getRecalPointFace(eTableBorder border)
{
    switch (border) {
        case eTableBorder_TOP_Y:
            return PointCap(m_pose.toAmbiPose(m_color).x, TABLE_TOP_Y - conf->xav(), 90);
            break;

        case eTableBorder_START_WALL_Y:
            return PointCap(m_pose.toAmbiPose(m_color).x, 618 - conf->xav(), 90);
            break;

        case eTableBorder_BOT_Y:
            return PointCap(m_pose.toAmbiPose(m_color).x, -TABLE_TOP_Y + conf->xav(), -90);
            break;

        case eTableBorder_OPP_SIDE_X:
            return PointCap(-TABLE_BORDER_X + conf->xav(), m_pose.toAmbiPose(m_color).y, 180);
            break;

        case eTableBorder_OWN_SIDE_X:
            return PointCap(TABLE_BORDER_X - conf->xav(), m_pose.toAmbiPose(m_color).y, 0);
            break;

        case eTableBorder_FLIP_FLOP_X:
            return PointCap(790 - conf->xav(), m_pose.toAmbiPose(m_color).y, 0);
            break;

        default:
            ASSERT(false);
            return PointCap();
            break;
    }
}

PointCap Navigation::getRecalPointRear(eTableBorder border)
{
    switch (border) {
        case eTableBorder_TOP_Y:
            return PointCap(m_pose.toAmbiPose(m_color).x, TABLE_TOP_Y - conf->xar(), -90);
            break;

        case eTableBorder_START_WALL_Y:
            return PointCap(m_pose.toAmbiPose(m_color).x, 618 - conf->xar(), -90);
            break;

        case eTableBorder_BOT_Y:
            return PointCap(m_pose.toAmbiPose(m_color).x, -TABLE_TOP_Y + conf->xar(), 90);
            break;

        case eTableBorder_OPP_SIDE_X:
            return PointCap(-TABLE_BORDER_X + conf->xar(), m_pose.toAmbiPose(m_color).y, 0);
            break;

        case eTableBorder_OWN_SIDE_X:
            return PointCap(TABLE_BORDER_X - conf->xar(), m_pose.toAmbiPose(m_color).y, 180);
            break;

        case eTableBorder_FLIP_FLOP_X:
            return PointCap(790 - conf->xar(), m_pose.toAmbiPose(m_color).y, 180);
            break;

        default:
            ASSERT(false);
            return PointCap();
            break;
    }
}

String Navigation::sensToString(eDir sens)
{
    switch (sens)
    {
    default:
        ASSERT(false);
        return "";
        break;
    ENUM2STR(eDir_BEST)
;        ENUM2STR(eDir_FORWARD);
        ENUM2STR(eDir_BACKWARD);
    }
}

String Navigation::orderToString(eNavOrder order)
{
    switch (order)
    {
    default:
        ASSERT(false);
        return "";
        break;
    ENUM2STR(eNavOrder_NOTHING)
;        ENUM2STR(eNavOrder_GOTO);
        ENUM2STR(eNavOrder_GOTO_CAP);
        ENUM2STR(eNavOrder_STOP);
        ENUM2STR(eNavOrder_RECAL_FACE);
        ENUM2STR(eNavOrder_RECAL_REAR);
        ENUM2STR(eNavOrder_GRAPH_TO);
    }
}

String Navigation::stateToString(eNavState state)
{
    switch (state)
    {
    default:
        ASSERT(false);
        return "";
        break;
    ENUM2STR(eNavState_IDLE)
;        ENUM2STR(eNavState_FACING_DEST);
        ENUM2STR(eNavState_GOING_TO_TARGET);
        ENUM2STR(eNavState_TURNING_AT_TARGET);
        ENUM2STR(eNavState_STOPPING);
        ENUM2STR(eNavState_WAIT_OPP_MOVE);
        ENUM2STR(eNavState_FACING_WALL);
        ENUM2STR(eNavState_CONTACTING_WALL);
        ENUM2STR(eNavState_ESCAPING_WALL);
        ENUM2STR(eNavState_WAIT_OPP_ESCAPE_RECALL);
        ENUM2STR(eNavState_BLOCKED);
        ENUM2STR(eNavState_STOPPING_IN_BLOCKED);
        ENUM2STR(eNavState_STOPPING_IN_WALL);
        ENUM2STR(eNavState_COMPUTING_GRAPH);
    }
}


