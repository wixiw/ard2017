#include <Arduino.h>
#include "core/ArdFramework.h"
#include "Log.h"
#include "Navigation.h"
#include "Actuators/Buzzer.h"
#include "Detection.h"
#include "Graph.h"
#include "KinematicManager.h"

using namespace ard;

/* Angle delta under which we consider target is reached 2.5mm on 5m */
#define NO_TURN_DELTA 0.0005 //rad
#define NO_MOVE_DELTA 1      //mm
#define KLAXON_FREQ 1000     //Hz
#define RECAL_FORCING_SLOW 200     //mm
#define RECAL_FORCING_FAST 200     //mm/s
#define RECAL_TIMEOUT 5000 //ms
#define RECAL_RETRY_ESCAPE_DIST 50 //mm
#define GRAPH_TIMEOUT 15000 //ms
#define OPP_IMPATIENCE_TIMEOUT 5000 //ms
#define CHECK_ONE_ORDER_AT_A_TIME() ASSERT_TEXT(!orderOngoing(), "Nav cannot do 2 orders at a time");
#define AVOID_MARGIN 50 //mm distance added to robot outer circle to consider an escape point as being on table
#define AVOID_BACK_DISTANCE 100 //mm distance used to go backward and let some space between robots.

Navigation::Navigation(Buzzer& klaxon, Detection& detection, Graph& graph, KinematicManager& kinMan):
	Thread("Nav", PRIO_NAVIGATION, STACK_NAVIGATION, PERIOD_NAVIGATION),
	switchRecalFL(BORDURE_AVG, 500, 50, true),
	switchRecalFR(BORDURE_AVD, 500, 50, true),
	switchRecalRC(BORDURE_ARC, 500, 50, true),
	simulated(true),
	m_pose(),
	m_state(eNavState_IDLE),
	m_target(),
	m_startPoint(),
	m_targetDir(eDir_BEST),
	m_order(eNavOrder_NOTHING),
	stepperL(AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR),
	stepperR(AccelStepper::DRIVER, PAPD_STEP, PAPD_DIR),
	m_color(eColor_PREF),
	m_mutex(),
	oldStepL(0),
	oldStepR(0),
	currentWayPoint(0),
	m_graphDir(eDir_BEST),
	state(),
	escapeDist(0),
	triedCount(0),
	klaxon(klaxon),
	detection(detection),
	graph(graph),
	kinematics(kinMan)
{
    state = apb_NavState_init_default;
}

/**---------------------------------
 * Thread interface
 ---------------------------------*/

void Navigation::init()
{
    ASSERT_CONFIGURED();
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
        applyCmdStop();
		m_state = eNavState_STOPPING_IN_BLOCKED;
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
            if (subOrderFinished())
            {
                //Request rotation to final heading
                action_turningAtTarget();
            }
            else
            {
                //check for opponent presence (if avoidance system is active)
                if( detection.isOpponentOnPath(m_targetDir, getPosition(SYM_POS)) )
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
            if( !detection.isOpponentOnPath(m_targetDir, getPosition(SYM_POS)) )
            {
                LOG_INFO("Opponent has moved away, continuing order.");
                action_goingToTarget();
            }
            //If opponent is still present, continue to wait
            //else
            //    LOG_DEBUG("Opponent is still present, continuing to wait.");
            break;
        }

        case eNavState_AVOID_GO_BACKWARD:
		{
			if( subOrderFinished() )
			{
//TODO test in progress
//				LOG_INFO("Room made for opponent.");
//				m_state = eNavState_WAIT_OPP_MOVE;
				action_avoidScan(true);
			}
			break;
		}

        case eNavState_AVOID_SCAN:
		{
			if( subOrderFinished() )
			{
				LOG_INFO("Scan finished, opponent is every where !."); //TODO : tester de l'autre côté
				m_state = eNavState_WAIT_OPP_MOVE;
			}
			else
			{
				/*TODO gere sens*/
				if(detection.omronFront.read() ==  GPIO_LOW)
				{
					LOG_INFO("Scan finished, stop turning, we find a hole.");
					applyCmdStop();
					m_state = eNavState_AVOID_SCAN_STOP;
				}
			}
			break;
		}

        case eNavState_AVOID_SCAN_STOP:
		{
			if( subOrderFinished() )
			{
				LOG_INFO("Contourning opponent");
				m_state = eNavState_WAIT_OPP_MOVE;
			}
			break;
		}



/** -----------------------------------------------------------------------------------
 * Recal on Border
 --------------------------------------------------------------------------------------*/

        case eNavState_FACING_WALL:
        {
            if (subOrderFinished())
            {
                LOG_INFO("   wall faced, contacting...");
                switch (m_order) {
                    case eNavOrder_RECAL_FACE:
                        //Request straight line
                        applyCmdToGoStraight(RECAL_FORCING_SLOW, conf->recalSpeed(), kinematics.maxAcc());
                        break;
                    case eNavOrder_RECAL_REAR:
                        //Request straight line
                        applyCmdToGoStraight(-RECAL_FORCING_SLOW, conf->recalSpeed(), kinematics.maxAcc());
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
            if (subOrderFinished())
            {
                LOG_INFO("   wall contacted, forcing...");
                switch (m_order) {
                    case eNavOrder_RECAL_FACE:
                        //Request straight line
                        applyCmdToGoStraight(RECAL_FORCING_FAST, kinematics.maxSpeed(m_targetDir), kinematics.maxAcc());
                        break;
                    case eNavOrder_RECAL_REAR:
                        //Request straight line
                        applyCmdToGoStraight(-RECAL_FORCING_FAST, kinematics.maxSpeed(m_targetDir), kinematics.maxAcc());
                        break;
                    default:
                        ASSERT(false); //should not be reached, dev bug
                        break;
                }
                m_state = eNavState_PUSHING_WALL;
            }
        	break;
        }

        case eNavState_PUSHING_WALL:
        {
            //If both switch are contacted (or target reached in no switch mode)
            if( (simulated  && subOrderFinished())
                  || (!simulated && m_order == eNavOrder_RECAL_FACE && switchRecalFL.read() && switchRecalFR.read())
                  || (!simulated && m_order == eNavOrder_RECAL_REAR && (subOrderFinished() /* TODO a virer quand switch AR remis || switchRecalRC.read()*/)))
            {
            	if( !subOrderFinished() )
            		LOG_INFO(String("   wall touched with switches"));
            	else
            		LOG_INFO(String("   wall touched (target reached)"));

                setPosition(m_target, false);//position has already been symetrized

                klaxon.bip(1);
                if(escapeDist != 0)
                {
					//Escape from wall
                	m_targetDir = computeRecalEscapdeDir();
					m_target.translatePolar(m_target.hDegree(), m_targetDir * escapeDist);
					applyCmdToGoStraight(m_targetDir * escapeDist, kinematics.maxSpeed(m_targetDir), kinematics.maxAcc());
					m_state = eNavState_ESCAPING_WALL;
					orderTimeout.arm(1000+OPP_IMPATIENCE_TIMEOUT);
					LOG_INFO(String("   escaping of distance=") + escapeDist + " to reach pose=" + m_target.toString()+".");
                }
                else
                {
                    LOG_INFO(String("   recal finished (no escape), exit position : ")+ getPosition(NO_SYM_POS).toString());
                    m_order = eNavOrder_NOTHING;
                    m_state = eNavState_IDLE;
                }
            }
            //If target is reached (in switch mode) then the recal failed
            else if(!simulated && m_order == eNavOrder_RECAL_FACE //TODO a virer quand switch AR remis
            		&& subOrderFinished() )
            {
            	if(triedCount < 1)
            	{
					triedCount++;
					m_targetDir = computeRecalEscapdeDir();
					applyCmdToGoStraight(m_targetDir * RECAL_RETRY_ESCAPE_DIST, kinematics.maxSpeed(m_targetDir), kinematics.maxAcc());
					LOG_ERROR("Failed to recal on border, escaped of " + String(m_targetDir * RECAL_RETRY_ESCAPE_DIST));
					m_state = eNavState_ESCAPE_WALL_FAILED;
					orderTimeout.arm(RECAL_TIMEOUT);
				}
				else
            	{
                	LOG_ERROR("Failed to recal on border, but still set position as it's would be worse not to do so.");
                	setPosition(m_target, false);//position has already been symetrized
                	klaxon.bip(5);
                	m_order = eNavOrder_NOTHING;
                	m_state = eNavState_IDLE;
            	}
            }
            break;
        }

        case eNavState_ESCAPE_WALL_FAILED:
        {
        	if(subOrderFinished())
        	{
        		LOG_INFO("   retry position reached, do it again...");
        		m_state = eNavState_FACING_WALL;
        	}
        	break;
        }

        case eNavState_ESCAPING_WALL:
        {
            //check for opponent presence (if avoidance system is active)
            if( (m_order == eNavOrder_RECAL_REAR && detection.isOpponentAhead(getPosition(SYM_POS)))
            || (m_order == eNavOrder_RECAL_FACE && detection.isOpponentBehind(getPosition(SYM_POS))) )
            {
                //Stops as an opponent is detected
            	applyCmdStop();

                m_state = eNavState_WAIT_OPP_ESCAPE_RECALL;
                LOG_INFO("       Opponent is blocking recal escape");
            }
            else if(subOrderFinished())
            {
                LOG_INFO(String("   recal finished, exit position : ")+ getPosition(NO_SYM_POS).toString());
                m_order = eNavOrder_NOTHING;
                m_state = eNavState_IDLE;
            }
            break;
        }

        //This avoidance case is particular as the robot is stucked against the wall, so it will try to move as soon as possible
        case eNavState_WAIT_OPP_ESCAPE_RECALL:
        {
            //check for opponent presence (if avoidance system is active)
            if( (m_order == eNavOrder_RECAL_REAR && detection.isOpponentAhead(getPosition(SYM_POS)))
            || (m_order == eNavOrder_RECAL_FACE && detection.isOpponentBehind(getPosition(SYM_POS))) )
            {
                klaxon.playTone(KLAXON_FREQ, PERIOD_NAVIGATION);
            }
            else
            {
                m_state = eNavState_ESCAPING_WALL;
                LOG_INFO("       Opponent moved away");

                //Reset order
                ASSERT(m_targetDir);
                applyCmdToGoStraight(m_targetDir * getPosition(NO_SYM_POS).distanceTo(m_target), kinematics.maxSpeed(m_targetDir), kinematics.maxAcc());
            }
            break;
        }

        /** -----------------------------------------------------------------------------------
         * GRAPH MANAGEMENT
         --------------------------------------------------------------------------------------*/
        case eNavState_COMPUTING_GRAPH:
        {
            if(!graph.computeShortertPath(getPosition(SYM_POS), m_target, m_graphDir))
            {
                LOG_ERROR("No path found in graph !");
                m_order = eNavOrder_NOTHING;
                m_state = eNavState_BLOCKED;
                return;
            }
            else
            {
            	//A state is dedicated to prevent holding the thread too long
            	m_state = eNavState_OPTIMIZING_GRAPH;
            }
            break;
        }

        case eNavState_OPTIMIZING_GRAPH:
        {
        	graph.optimizePath(m_graphDir);

            //Go to first point
            currentWayPoint = 0;
            action_gotoNextWaypoint();

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

void Navigation::motorPower(bool on)
{
	if(on)
		digitalWrite(PAP_ENABLE, LOW);
	else
		digitalWrite(PAP_ENABLE, HIGH);
};

void Navigation::setPosition(Pose2D newPose, bool sym)
{
    //prevent any interrupt from occurring so that position fields are not corrupted
    enterCriticalSection();

    stepperL.setCurrentPosition(0);
    oldStepL = 0;
    stepperL.move(0); //move(0) is equivalent to stay on current position
    stepperR.setCurrentPosition(0);
    oldStepR = 0;
    stepperR.move(0);

    if(sym)
        m_pose = newPose.toAmbiPose(m_color);
    else
        m_pose = newPose;
    exitCriticalSection();

    LOG_INFO("   position set to :" + newPose.toString());
}


void Navigation::goTo(Point target, eDir sens, bool sym)
{
    m_mutex.lock();
    CHECK_ONE_ORDER_AT_A_TIME();
    triedCount = 0;
    motorPower(true);
    m_order = eNavOrder_GOTO;

    //Symetrize target
    if(sym)
        m_target = target.toAmbiPoint(m_color);
    else
        m_target = target;
    m_startPoint = getPosition(NO_SYM_POS);

    //Choose direction
    DelayMs moveDur = 0;
    if(sens == eDir_BEST)
    {
        m_targetDir = findOptimalDir(getPosition(NO_SYM_POS), m_target, false, &moveDur);
    }
    else
    {
        moveDur = motionDuration(getPosition(NO_SYM_POS), m_target, sens, false);
        m_targetDir = sens;
    }

    LOG_INFO("   new request : goTo" + m_target.toString() + " "  + sensToString(sens) + ".");
    action_startOrder();

    orderTimeout.arm(moveDur + OPP_IMPATIENCE_TIMEOUT);

    m_mutex.unlock();
}

void Navigation::goToCap(Pose2D target, eDir sens, bool sym)
{
    m_mutex.lock();
    CHECK_ONE_ORDER_AT_A_TIME();
    triedCount = 0;
    motorPower(true);
    m_order = eNavOrder_GOTO_CAP;

    //Symetrize target
    if(sym)
        m_target = target.toAmbiPose(m_color);
    else
        m_target = target;
    m_startPoint = getPosition(NO_SYM_POS);

    //Choose direction
    DelayMs moveDur = 0;
    if(sens == eDir_BEST)
    {
        m_targetDir = findOptimalDir(getPosition(NO_SYM_POS), m_target, true, &moveDur);
    }
    else
    {
        moveDur = motionDuration(getPosition(NO_SYM_POS), m_target, sens, true);
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
    Pose2D target = getPosition(NO_SYM_POS);
    target.translatePolar(m_pose.hDegree(), distanceMm);
    m_mutex.unlock();

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
    m_mutex.lock();
    CHECK_ONE_ORDER_AT_A_TIME();
    triedCount = 0;
    motorPower(true);
    m_startPoint = getPosition(NO_SYM_POS);

    LOG_INFO(String("   new request : turnDelta(") + angle + "°).");
    float target = angle;

    if(sym && m_color == eColor_SYM)
        target = -angle;
    else
        target = angle;

    //Request turn
    applyCmdToTurn(radians(target), kinematics.maxTurnSpeed(), kinematics.maxTurnAcc());
    orderTimeout.arm(2* fabs(target)*1000. / (double)kinematics.maxTurnSpeed());

    //Change state
    m_order = eNavOrder_GOTO_CAP;
    m_state = eNavState_TURNING_AT_TARGET;

    m_mutex.unlock();
}

void Navigation::turnTo(float angle, bool sym)
{
    LOG_INFO(String("   new request : turnTo(") + angle + "°).");
    Pose2D target = getPosition(NO_SYM_POS);
    m_startPoint = getPosition(NO_SYM_POS);

    if(sym && m_color == eColor_SYM)
        target.hDegree(-angle);
    else
        target.hDegree(angle);

    goToCap(target, eDir_FORWARD, false);
}

void Navigation::faceTo(Point p, bool sym)
{
    LOG_INFO(String("   new request : faceTo") + p.toString() + ".");
    m_startPoint = getPosition(NO_SYM_POS);
    Pose2D target = m_startPoint;

    if( sym )
        target.h = m_startPoint.angleTo(p.toAmbiPoint(m_color));
    else
        target.h = m_startPoint.angleTo(p);

    goToCap(target, eDir_FORWARD, true);
}

void Navigation::rearTo(Point p, bool sym)
{
    LOG_INFO(String("   new request : rearTo") + p.toString() + ".");
    m_startPoint = getPosition(NO_SYM_POS);
    Pose2D target = m_startPoint;

    if( sym )
        target.h = headingToDir(m_startPoint.angleTo(p.toAmbiPoint(m_color)), eDir_BACKWARD);
    else
        target.h =headingToDir(m_startPoint.angleTo(p), eDir_BACKWARD);

    goToCap(target, eDir_BACKWARD, true);
}

void Navigation::recalFace(eTableBorder border, Distance _escapeDir)
{
    LOG_INFO(String("   new request : recalFace on border=") + border + " with escape = " + String(escapeDist));

    m_mutex.lock();
    CHECK_ONE_ORDER_AT_A_TIME()
    motorPower(true);
    m_order = eNavOrder_RECAL_FACE;
    m_targetDir = eDir_FORWARD;
    triedCount = 0;
    m_target = getRecalPointFace(border).toAmbiPose(m_color);
    double angleDelta = moduloPiPi(m_target.h - getPosition(NO_SYM_POS).h);

    //Request turn
    LOG_INFO(String("   Facing wall to recal at ") + m_target.toString() + "...");
    applyCmdToTurn(angleDelta, kinematics.maxSpeed(m_targetDir), kinematics.maxAcc()); //We will go to reset position, so let do it quickly ^^
    m_state = eNavState_FACING_WALL;

    orderTimeout.arm(RECAL_TIMEOUT);

    escapeDist = _escapeDir;

    m_mutex.unlock();
}

void Navigation::recalRear(eTableBorder border, Distance _escapeDir)
{
    LOG_INFO(String("   new request : recalRear on border=") + border + " with escape = " + String(escapeDist));

    m_mutex.lock();
    CHECK_ONE_ORDER_AT_A_TIME()
    motorPower(true);
    m_order = eNavOrder_RECAL_REAR;
    m_targetDir = eDir_BACKWARD;
    triedCount = 0;
    m_target = getRecalPointRear(border).toAmbiPose(m_color);;
    double angleDelta = moduloPiPi(m_target.h - getPosition(NO_SYM_POS).h);

    //Request turn
    LOG_INFO(String("   Facing table to recal at ") + m_target.toString() + "...");
    applyCmdToTurn(angleDelta, kinematics.maxSpeed(m_targetDir), kinematics.maxAcc()); //We will go to reset position, so let do it quickly ^^
    m_state = eNavState_FACING_WALL;

    orderTimeout.arm(RECAL_TIMEOUT);

    escapeDist = _escapeDir;

    m_mutex.unlock();
}

void Navigation::graphTo(Pose2D target, eDir sens)
{
    m_mutex.lock();
    CHECK_ONE_ORDER_AT_A_TIME();
    triedCount = 0;
    motorPower(true);
    LOG_INFO(String("   new request :  graphTo ") + target.toString() + ", computation in progress...");

    //Computation is delayed for a further time to prevent the caller from making the graph search
    m_target = target;
    m_state = eNavState_COMPUTING_GRAPH;
    m_targetDir = sens;
    m_graphDir = sens;
    m_order = eNavOrder_GRAPH_TO;
    orderTimeout.arm(GRAPH_TIMEOUT);
    m_startPoint = getPosition(NO_SYM_POS);
    m_mutex.unlock();
}

void Navigation::stopMoving()
{
	if(orderOngoing())
		LOG_INFO("Strategy requested to interrupt the current order.");
	else
		LOG_INFO("Strategy requested a stop, but no move is ongoing : ignored.");

    m_mutex.lock();
    applyCmdStop();
    m_order = eNavOrder_NOTHING;
	m_state = eNavState_STOPPING;
    m_mutex.unlock();
}

bool Navigation::orderOngoing()
{
	return !((m_state == eNavState_IDLE || m_state == eNavState_BLOCKED) && m_order == eNavOrder_NOTHING);
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

DelayMs Navigation::motionDuration(Pose2D const& start, Pose2D const& end, eDir sens, bool isGotoCap)
{
    double startAngle = 0;
    double endAngle = 0;

    //Determine start and stop angles
    startAngle = degrees(fabs(headingToDir(start.angleHeadingTo(end), sens)));
    if(isGotoCap)
        endAngle = degrees(fabs(headingToDir(start.angleTo(end) - end.h, sens)));

    //compute durations
    DelayMs translationDuration = start.distanceTo(end)*1000./kinematics.maxSpeed(sens);
    DelayMs rotationStartDuration = startAngle*1000. / (double)kinematics.maxTurnSpeed();
    DelayMs rotationEndDuration = endAngle*1000. / (double)kinematics.maxTurnSpeed();
    DelayMs duration = translationDuration + rotationStartDuration + rotationEndDuration;

    LOG_DEBUG(String("Order duration is : ") + duration + "ms (T="+ translationDuration + " Rs=" + rotationStartDuration + " Re=" + rotationEndDuration+")");

    return duration;
}

eDir Navigation::findOptimalDir(Pose2D const& start, Pose2D const& end, bool isGotoCap, DelayMs* duration)
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

eDir Navigation::computeRecalEscapdeDir()
{
	if(m_order == eNavOrder_RECAL_FACE)
	{
		return eDir_BACKWARD;
	}
	else if(m_order == eNavOrder_RECAL_REAR)
	{
		return eDir_FORWARD;
	}
	else
	{
		ASSERT(false);
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
    state.pos = getPosition(NO_SYM_POS).getProto();
    state.omronFront 		= detection.omronFront.readRaw();
    state.omronRear 		= detection.omronRear.readRaw();
	state.omronLatLeft 		= detection.omronLatLeft.readRaw();
	state.omronLatRight 	= detection.omronLatRight.readRaw();
	state.omronScan 		= detection.omronScan.readRaw();
    state.switchRecalFL 	= switchRecalFL.readRaw();
    state.switchRecalFR 	= switchRecalFR.readRaw();
    state.switchRecalRC 	= switchRecalRC.readRaw();

    state.vmax = kinematics.maxSpeed(m_targetDir);

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
    Pose2D currentPose = getPosition(NO_SYM_POS);
    double distDelta = m_targetDir * currentPose.distanceTo(m_target);
    //Do no move if already on target
    if (fabs(distDelta) <= NO_MOVE_DELTA)
    {
        action_turningAtTarget();
    }
    else
    {
        double angleDelta = headingToDir(currentPose.angleHeadingTo(m_target), m_targetDir);

        //Do not turn if already facing right direction
        if (fabs(angleDelta) <= NO_TURN_DELTA)
        {
            //Request straight line
            action_goingToTarget();
        }
        else
        {
            //Request turn
            applyCmdToTurn(angleDelta, kinematics.maxTurnSpeed(), kinematics.maxTurnAcc());
            //Change state
            m_state = eNavState_FACING_DEST;
        }
    }
}

void Navigation::action_goingToTarget()
{
    //Request straight line
    ASSERT(m_targetDir);
    applyCmdToGoStraight(m_targetDir * getPosition(NO_SYM_POS).distanceTo(m_target), kinematics.maxSpeed(m_targetDir), kinematics.maxAcc());
    //Change state
    m_state = eNavState_GOING_TO_TARGET;
}

void Navigation::action_turningAtTarget()
{
    //Request rotation to final heading
    if( m_order == eNavOrder_GOTO_CAP || m_order == eNavOrder_GRAPH_TO )
    {
        double angleDelta = moduloPiPi(m_target.h - getPosition(NO_SYM_POS).h);
        //Do not turn if already facing right direction
        if (fabs(angleDelta) <= NO_TURN_DELTA)
        {
            //Change state to terminate order
            action_finishOrder();
        }
        else
        {
            //Request turn
            applyCmdToTurn(angleDelta, kinematics.maxTurnSpeed(), kinematics.maxTurnAcc());
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
        m_targetDir = findOptimalDir(getPosition(NO_SYM_POS), m_target, eNavOrder_GOTO_CAP);
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
            break;

        case eNavOrder_GOTO_CAP:
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
                graph.reset();
            }
            break;
    }

    
    m_state = eNavState_IDLE;
    m_order = eNavOrder_NOTHING;
    LOG_INFO("   order finished.");
}

void Navigation::action_waitOppMove()
{
	Pose2D currentPose = getPosition(NO_SYM_POS);

    //Check if we can replay
    Distance distanceRun = m_startPoint.distanceTo(currentPose);
    if( AVOID_BACK_DISTANCE < distanceRun )
    {
    	LOG_INFO(String("Replaying  ") + (int)(AVOID_BACK_DISTANCE) + "mm (run distance=" + distanceRun + ")");
		m_state = eNavState_AVOID_GO_BACKWARD;
		applyCmdToGoStraight(- m_targetDir * AVOID_BACK_DISTANCE, kinematics.maxSpeed(m_targetDir), kinematics.maxAcc());
    	return;
    }

	//If trajectory was not on path, try to go back ahead of start pos
	Pose2D backPose = currentPose;
	backPose.translatePolar(currentPose.hDegree(), - m_targetDir * AVOID_BACK_DISTANCE);
	if( detection.isWaypointValid(backPose.toAmbiPose(m_color), conf->xouter() + AVOID_MARGIN))
	{
		LOG_INFO("Backing a bit to let some room to the opponent : " + backPose.toString());
		m_state = eNavState_AVOID_GO_BACKWARD;
		applyCmdToGoStraight(- m_targetDir * AVOID_BACK_DISTANCE, kinematics.maxSpeed(m_targetDir), kinematics.maxAcc());
		return;
	}

	if( 0 < distanceRun )
	{
    	LOG_INFO(String("Half replay : ") + distanceRun + "mm.");
		m_state = eNavState_AVOID_GO_BACKWARD;
		applyCmdToGoStraight(- m_targetDir * distanceRun, kinematics.maxSpeed(m_targetDir), kinematics.maxAcc());
    	return;
	}

	//If no solution is found : wait
	LOG_INFO("No backing solution : stops and wait that opponent moves away... (blocked at position = " + currentPose.toString()+")");
	applyCmdStop();
	m_state = eNavState_WAIT_OPP_MOVE;
	klaxon.playTone(KLAXON_FREQ, conf->detectionWaitForOppMove()); //Detection wait is managed by omron filters
}

void Navigation::action_avoidScan(bool left)
{
	double angleDelta = 0;

	if(left)
	{
		LOG_INFO("Scanning left direction...");
		angleDelta = M_PI_2;
	}
	else
	{
		LOG_INFO("Scanning right direction..");
		angleDelta = -M_PI_2;
	}

	m_state = eNavState_AVOID_SCAN;
	applyCmdToTurn(angleDelta, kinematics.maxTurnSpeed(), kinematics.maxTurnAcc());
	klaxon.bip(2);
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

void Navigation::applyCmdStop()
{
    LOG_INFO("applyCmdStop.");

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

Pose2D Navigation::getRecalPointFace(eTableBorder border)
{
    switch (border) {
        case eTableBorder_REFEREE_Y:
            return Pose2D(getPosition(SYM_POS).x, TABLE_BORDER_Y - conf->xav(), 90);
            break;

        case eTableBorder_B_CORNER_Y:
            return Pose2D(getPosition(SYM_POS).x, 618 - conf->xav(), 90);
            break;

        case eTableBorder_BOT_Y:
            return Pose2D(getPosition(SYM_POS).x, -TABLE_BORDER_Y + conf->xav(), -90);
            break;

        case eTableBorder_OPP_B_CORNER_X:
            return Pose2D(-TABLE_BORDER_X + conf->xav(), getPosition(SYM_POS).y, 180);
            break;

        case eTableBorder_OWN_B_CORNER_X:
            return Pose2D(TABLE_BORDER_X - conf->xav(), getPosition(SYM_POS).y, 0);
            break;

        case eTableBorder_FLIP_FLOP_X:
            return Pose2D(790 - conf->xav(), getPosition(SYM_POS).y, 0);
            break;

        case eTableBorder_OWN_BORDER_3_X:
        	return Pose2D(68 + conf->xav(), getPosition(SYM_POS).y, 180);
        	break;

        case eTableBorder_OPP_BORDER_3_X:
        	return Pose2D(-68 - conf->xav(), getPosition(SYM_POS).y, 0);
        	break;

        case eTableBorder_OWN_BORDER_5_X:
        	return Pose2D(TABLE_BORDER_X - 108 - conf->xav(), getPosition(SYM_POS).y, 0);
        	break;

        case eTableBorder_OPP_BORDER_1_X:
        	return Pose2D(-TABLE_BORDER_X + 108 + conf->xav(), getPosition(SYM_POS).y, 180);
        	break;

        default:
            ASSERT(false);
            return Pose2D();
            break;
    }
}

Pose2D Navigation::getRecalPointRear(eTableBorder border)
{
    switch (border) {
        case eTableBorder_REFEREE_Y:
            return Pose2D(getPosition(SYM_POS).x, TABLE_BORDER_Y - conf->xar(), -90);
            break;

        case eTableBorder_B_CORNER_Y:
            return Pose2D(getPosition(SYM_POS).x, 618 - conf->xar(), -90);
            break;

        case eTableBorder_BOT_Y:
            return Pose2D(getPosition(SYM_POS).x, -TABLE_BORDER_Y + conf->xar(), 90);
            break;

        case eTableBorder_OPP_B_CORNER_X:
            return Pose2D(-TABLE_BORDER_X + conf->xar(), getPosition(SYM_POS).y, 0);
            break;

        case eTableBorder_OWN_B_CORNER_X:
            return Pose2D(TABLE_BORDER_X - conf->xar(), getPosition(SYM_POS).y, 180);
            break;

        case eTableBorder_FLIP_FLOP_X:
            return Pose2D(790 - conf->xar(), getPosition(SYM_POS).y, 180);
            break;

        case eTableBorder_OWN_BORDER_3_X:
        	return Pose2D(-68 - conf->xar(), getPosition(SYM_POS).y, 0);
        	break;

        case eTableBorder_OPP_BORDER_3_X:
        	return Pose2D(68 + conf->xar(), getPosition(SYM_POS).y, 180);
        	break;

        case eTableBorder_OWN_BORDER_5_X:
        	return Pose2D(TABLE_BORDER_X - 108 - conf->xar(), getPosition(SYM_POS).y, 180);
        	break;

        case eTableBorder_OPP_BORDER_1_X:
        	return Pose2D(-TABLE_BORDER_X + 108 + conf->xar(), getPosition(SYM_POS).y, 0);
        	break;

        default:
            ASSERT(false);
            return Pose2D();
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
		ENUM2STR(eNavState_PUSHING_WALL);
        ENUM2STR(eNavState_ESCAPING_WALL);
        ENUM2STR(eNavState_ESCAPE_WALL_FAILED);
        ENUM2STR(eNavState_WAIT_OPP_ESCAPE_RECALL);
        ENUM2STR(eNavState_BLOCKED);
        ENUM2STR(eNavState_STOPPING_IN_BLOCKED);
        ENUM2STR(eNavState_STOPPING_IN_WALL);
        ENUM2STR(eNavState_COMPUTING_GRAPH);
        ENUM2STR(eNavState_OPTIMIZING_GRAPH);
        ENUM2STR(eNavState_AVOID_GO_BACKWARD);
        ENUM2STR(eNavState_AVOID_SCAN);
        ENUM2STR(eNavState_AVOID_SCAN_STOP);
    }
}


