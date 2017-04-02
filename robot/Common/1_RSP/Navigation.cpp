#include <Arduino.h>
#include "K_constants.h"
#include "Log.h"
#include "Navigation.h"

using namespace ard;

/* Angle delta under which we consider target is reached 2.5mm on 5m */
#define NO_TURN_DELTA 0.0005
#define NO_MOVE_DELTA 1

Navigation::Navigation()
        :
                Thread("Nav", PRIO_NAVIGATION, STACK_NAVIGATION, PERIOD_NAVIGATION),
                fakeRobot(false),
                m_pose(),
                m_state(eNavState_IDLE),
                m_target(),
                m_sensTarget(eDir_UNDEFINED),
                m_order(eNavOrder_NOTHING),
                stepperL(AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR),
                stepperR(AccelStepper::DRIVER, PAPD_STEP, PAPD_DIR),
                omronFrontLeft(OMRON1, 50, 50),
                omronFrontRight(OMRON2, 50, 50),
                omronRearLeft(OMRON3, 50, 50),
                omronRearRight(OMRON4, 50, 50),
                m_color(eColor_PREF),
                m_speed(0),
                m_speed_virage(0),
                m_mutex(),
                m_targetReached(),
                oldStepL(0),
                oldStepR(0),
                oppTimer(),
                avoidanceActive(false),//start desactivated, so that activation is done with start, ensuring avoidance is unactivated in simulation
                conf(NULL)
{

}

void Navigation::updateConf(RobotConfig* newConf)
{
    ASSERT(newConf);
    conf = newConf;
    m_speed = conf->maxSpeed();
    m_speed_virage = conf->maxTurnSpeed();
    stepperL.setAcceleration(fabs(conf->maxAcc() * conf->GAIN_MM_2_STEPS_LEFT));
    stepperR.setAcceleration(fabs(conf->maxAcc() * conf->GAIN_MM_2_STEPS_RIGHT));
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

    switch (m_state)
    {
        default:
        case eNavState_IDLE:
        {
            switch (m_order)
            {
                default:
                case eNavOrder_NOTHING:
                {
                    break;
                }

                case eNavOrder_GOTO:
                case eNavOrder_GOTO_CAP:
                {
                    action_startOrder();
                    break;
                }
            }
            break;
        }

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
                if( (m_sensTarget == eDir_FORWARD && isOpponentAhead())
                || (m_sensTarget == eDir_BACKWARD && isOpponentBehind()) )
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

        case eNavState_WAIT_OPP_MOVE:
        {
            if(oppTimer.isFired() || (!avoidanceActive  && !fakeRobot))
            {
                //Check if opponent has left
                if( (m_sensTarget == eDir_FORWARD && !isOpponentAhead())
                || (m_sensTarget == eDir_BACKWARD && !isOpponentBehind()) )
                {
                    LOG_INFO("Opponent has moved away, continuing order.");
                    LOG_DEBUG(String("Speed=") + m_speed + " togoL=" + stepperL.distanceToGo() + " togoR=" + stepperR.distanceToGo());
                    action_goingToTarget();
                }
                //If opponent is still present, continue to wait
                else
                {
                    oppTimer.arm(conf->detectionWaitForOppMove());
                }
            }

            break;
        }
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

void Navigation::setPosition(PointCap newPose)
{
    //prevent any interrupt from occurring so that position fields are not corrupted
    enterCriticalSection();
    m_pose = newPose.toAmbiPose(m_color);
    exitCriticalSection();

    LOG_INFO("position set to :" + newPose.toString());
}

void Navigation::goTo(Point target, eDir sens)
{
    m_mutex.lock();

    if (m_state != eNavState_IDLE || m_order != eNavOrder_NOTHING)
    {
        LOG_INFO("new order pending until current order is finished");
        m_mutex.unlock();
        wait();
        m_mutex.lock();
    }

    m_order = eNavOrder_GOTO;
    m_target = target.toAmbiPose(m_color);
    m_sensTarget = sens;
    action_startOrder();

    m_mutex.unlock();
}

void Navigation::goToCap(PointCap target, eDir sens)
{
    m_mutex.lock();

    //If an order is present, wait
    if (m_state != eNavState_IDLE || m_order != eNavOrder_NOTHING)
    {
        LOG_INFO("new order pending until current order is finished");
        m_mutex.unlock();
        wait();
        m_mutex.lock();
    }

    m_order = eNavOrder_GOTO_CAP;
    m_target = target.toAmbiPose(m_color);
    m_sensTarget = sens;
    action_startOrder();

    m_mutex.unlock();
}

void Navigation::goForward(float distanceMm)
{
    ASSERT_TEXT(false, "not implemented");
    m_mutex.lock();

    //If an order is present, wait
    if (m_state != eNavState_IDLE || m_order != eNavOrder_NOTHING)
    {
        LOG_INFO("new order pending until current order is finished");
        m_mutex.unlock();
        wait();
        m_mutex.lock();
    }

    //TODO
//  m_order = XXX;
//  m_target = target.toAmbiPose(m_color);
//  m_sensTarget = sens;

    m_mutex.unlock();
}

void Navigation::turnTo(float angle)
{
    ASSERT_TEXT(false, "not implemented");
    m_mutex.lock();

    //If an order is present, wait
    if (m_state != eNavState_IDLE || m_order != eNavOrder_NOTHING)
    {
        LOG_INFO("new order pending until current order is finished");
        m_mutex.unlock();
        wait();
        m_mutex.lock();
    }

    //TODO
//  m_order = XXX;
//  m_target = target.toAmbiPose(m_color);
//  m_sensTarget = sens;

    m_mutex.unlock();
}

void Navigation::faceTo(Point p)
{
    ASSERT_TEXT(false, "not implemented");
    m_mutex.lock();

    //If an order is present, wait
    if (m_state != eNavState_IDLE || m_order != eNavOrder_NOTHING)
    {
        LOG_INFO("new order pending until current order is finished");
        m_mutex.unlock();
        wait();
        m_mutex.lock();
    }

    //TODO
//  m_order = XXX;
//  m_target = target.toAmbiPose(m_color);
//  m_sensTarget = sens;

    m_mutex.unlock();
}

void Navigation::stopMoving()
{
    LOG_INFO("stop requested");
    m_mutex.lock();

    //prevent any interrupt from occurring between any configuration of a left/right motor
    enterCriticalSection();
    stepperL.stop();
    stepperR.stop();
    exitCriticalSection();

    //The state is directly changed to interrupting
    m_state = eNavState_STOPPING;
    m_order = eNavOrder_NOTHING;

    m_mutex.unlock();
}

void Navigation::wait()
{
    //obviously don't put a mutex, it's a blocking call ... !
    m_targetReached.wait();
}

bool Navigation::targetReached()
{
    m_mutex.lock();
    bool res = m_state == eNavState_IDLE;
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

void Navigation::setSpeed(float s)
{
    //prevent any interrupt from occurring between any configuration of a left/right motor
    enterCriticalSection();
    if (s > 0)
    {
        stepperL.setMaxSpeed(fabs(s * conf->GAIN_MM_2_STEPS_LEFT));
        stepperR.setMaxSpeed(fabs(s * conf->GAIN_MM_2_STEPS_RIGHT));
        m_speed = s;
    }
    else
    {
        stepperL.setMaxSpeed(fabs(conf->maxSpeed() * conf->GAIN_MM_2_STEPS_LEFT));
        stepperR.setMaxSpeed(fabs(conf->maxSpeed() * conf->GAIN_MM_2_STEPS_RIGHT));
        m_speed = conf->maxSpeed();
    }
    exitCriticalSection();
}

void Navigation::setSpeedVir(float s)
{
    m_mutex.lock();
    if (s > 0)
    {
        m_speed_virage = s;
    }
    else
    {
        m_speed_virage = conf->maxTurnSpeed();
    }
    m_mutex.unlock();
}

/**---------------------------------
 * Publish state
 ---------------------------------*/
apb_NavState Navigation::getState() const
{
    apb_NavState state = apb_NavState_init_default;

    state.state = m_state;
    state.order = m_order;
    state.pos = m_pose.getProto();

    state.omronFL = omronFrontLeft.readRaw();
    state.omronFR = omronFrontRight.readRaw();
    state.omronRL = omronRearLeft.readRaw();
    state.omronRR = omronRearRight.readRaw();

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
    LOG_INFO("new order " + orderToString(m_order) + "(" + m_target.x + ", " + m_target.y + ", " + m_target.h + ") " + sensToString(m_sensTarget) + ".");
    double angleToTarget = atan2((m_target.y - m_pose.y), (m_target.x - m_pose.x));
    if (m_sensTarget == eDir_BACKWARD)
    {
        angleToTarget = moduloPiPi(angleToTarget + M_PI);
    }
    double angleDelta = moduloPiPi(angleToTarget - m_pose.h);
    //Do not turn if already facing right direction
    if (fabs(angleDelta) <= NO_TURN_DELTA)
    {
        double distDelta = m_sensTarget * m_pose.distanceTo(m_target);
        //Do no move if already on target
        if (fabs(distDelta) <= NO_MOVE_DELTA)
        {
            action_turningAtTarget();
        }
        else
        {
            //Request straight line
            action_goingToTarget();
        }
    }
    else
    {
        //Request turn
        applyCmdToTurn(angleDelta);
        //Change state
        m_state = eNavState_FACING_DEST;
    }
}

void Navigation::action_goingToTarget()
{
    //Request straight line
    applyCmdToGoStraight(m_sensTarget * m_pose.distanceTo(m_target));
    //Change state
    m_state = eNavState_GOING_TO_TARGET;
}

void Navigation::action_turningAtTarget()
{
    //Request rotation to final heading
    if (m_order == eNavOrder_GOTO_CAP)
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
            applyCmdToTurn(angleDelta);
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


void Navigation::action_finishOrder()
{
    //As move is expected to be perfect, correct any numerical instability by forcing position to be exactly the target
    enterCriticalSection();
    switch(m_order)
    {
        default:
        case eNavOrder_NOTHING:
            ASSERT(false);
            break;

        case eNavOrder_GOTO:
            m_pose.x = m_target.x;
            m_pose.y = m_target.y;
            break;

        case eNavOrder_GOTO_CAP:
            m_pose.x = m_target.x;
            m_pose.y = m_target.y;
            m_pose.h = m_target.h;
            break;
    }
    exitCriticalSection();
    
    m_state = eNavState_IDLE;
    m_order = eNavOrder_NOTHING;
    m_targetReached.set();
    LOG_INFO("order finished.");
}

void Navigation::action_waitOppMove()
{
    enterCriticalSection();
    stepperL.stop();
    stepperR.stop();
    exitCriticalSection();

    oppTimer.arm(conf->detectionWaitForOppMove());
    m_state = eNavState_WAIT_OPP_MOVE;
    LOG_INFO("Waiting that opponent moves away...");
}

void Navigation::applyCmdToGoStraight(double mm)
{
    LOG_DEBUG(String("applyCmdToGoStraight : ") + mm + " mm");

    //prevent any interrupt from occurring between any configuration of a left/right motor
    enterCriticalSection();
    stepperL.setMaxSpeed(fabs(m_speed * conf->GAIN_MM_2_STEPS_LEFT));
    stepperR.setMaxSpeed(fabs(m_speed * conf->GAIN_MM_2_STEPS_RIGHT));
    stepperL.move(mm * conf->GAIN_MM_2_STEPS_LEFT);
    stepperR.move(mm * conf->GAIN_MM_2_STEPS_RIGHT);
    exitCriticalSection();
}

void Navigation::applyCmdToTurn(double angleInRad)
{
    LOG_DEBUG(String("applyCmdToTurn : ") + angleInRad + " rad");

    //prevent any interrupt from occurring between any configuration of a left/right motor
    enterCriticalSection();
    stepperL.setMaxSpeed(fabs(m_speed_virage * conf->GAIN_DEG_2_MM_LEFT));
    stepperR.setMaxSpeed(fabs(m_speed_virage * conf->GAIN_DEG_2_MM_RIGHT));
    stepperL.move(-angleInRad * conf->GAIN_RAD_2_MM_LEFT/2.); //half contribution on each wheel
    stepperR.move(angleInRad * conf->GAIN_RAD_2_MM_RIGHT/2.);
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

bool Navigation::isOpponentAhead()
{
    if( fakeRobot ||
        (avoidanceActive && (omronFrontLeft.read() == GPIO_HIGH || omronFrontRight.read() == GPIO_HIGH)) )
    {
        LOG_DEBUG(String("Opponent ahead : L=") + omronFrontLeft.read() + " R=" + omronFrontRight.read()
                + " Lraw=" + omronFrontLeft.readRaw() + " Rraw=" + omronFrontRight.readRaw());
        return true;
    }
    else
        return false;
}

bool Navigation::isOpponentBehind()
{
    if( fakeRobot ||
        (avoidanceActive && (omronRearLeft.read() == GPIO_HIGH || omronRearRight.read() == GPIO_HIGH)) )
    {
        LOG_DEBUG(String("Opponent behind : L=") + omronRearLeft.read() + " R=" + omronRearRight.read());
        return true;
    }
    else
        return false;
}

String Navigation::sensToString(eDir sens)
{
    switch (sens)
    {
    default:
        ASSERT(false);
        return "";
        break;
    ENUM2STR(eDir_UNDEFINED)
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
    }
}

void ard::Navigation::enableAvoidance(bool on)
{
    if( on && !avoidanceActive )
        LOG_INFO("Avoidance system activated.");
    if( !on && avoidanceActive )
        LOG_INFO("Avoidance system deactivated.");

    avoidanceActive = on;
}
