#include <Arduino.h>
#include "K_constants.h"
#include "BSP.h"
#include "Log.h"
#include "Navigation.h"

using namespace ard;

Navigation::Navigation():
    Thread("Nav", PRIO_NAVIGATION, STACK_NAVIGATION, PERIOD_NAVIGATION),
    m_pose(),
    m_state(eNavState_IDLE),
    m_target(),
    m_sensTarget(eDir_UNDEFINED),
    m_order(eNavOrder_NOTHING),
    m_angleToTarget(0),
    m_distanceToTarget(0),
    stepperG(AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR),
    stepperD(AccelStepper::DRIVER, PAPD_STEP, PAPD_DIR),
    omronFrontLeft (OMRON1, 50, 50, FilteredInput::INVERTED),
    omronFrontRight(OMRON2, 50, 50, FilteredInput::INVERTED),
    omronRearLeft  (OMRON3, 50, 50, FilteredInput::INVERTED),
    omronRearRight (OMRON4, 50, 50, FilteredInput::INVERTED),
    m_color(eColor_PREF),
    m_speed(SPEED_MAX),
    m_speed_virage(SPEED_MAX_VIR),
    m_mutex(),
    m_targetReached(),
    oldStepG(0),
    oldStepD(0)
{
    stepperG.setAcceleration(ACC_MAX);
    stepperD.setAcceleration(ACC_MAX);
}

/**---------------------------------
 * Thread interface
 ---------------------------------*/

void Navigation::run()
{
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
                LOG_INFO(
                        "new order " + orderToString(m_order) + "(" + m_target.x + ", " + m_target.y + ", " + m_target.h + ") " + sensToString(m_sensTarget)
                                + ".");
                //TODO et en marche AR ?
                double relTargetHeading = atan((m_target.y - m_pose.y)/(m_target.x - m_pose.x));
                if (relTargetHeading - m_pose.h < -M_PI)
                    m_angleToTarget = relTargetHeading - m_pose.h + 2 * M_PI;
                else if (relTargetHeading - m_pose.h > M_PI)
                    m_angleToTarget = relTargetHeading - m_pose.h - 2 * M_PI;
                else
                    m_angleToTarget = relTargetHeading - m_pose.h;
                turn(m_angleToTarget);
                m_state = eNavState_FACING_DEST;
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
                m_distanceToTarget = m_sensTarget * m_pose.distanceTo(m_target);
                straight(m_distanceToTarget);

                //Change state
                m_angleToTarget = 0;
                m_state = eNavState_GOING_TO_TARGET;
                LOG_DEBUG("target destination heading reached, beginning line to reach target.");
            }
            break;
        }

        case eNavState_GOING_TO_TARGET:
        {
            if (subOrderFinished())
            {
                //Request rotation to final heading
                if (m_order == eNavOrder_GOTO_CAP)
                {
                    if (m_target.h - m_pose.h < -M_PI)
                        m_angleToTarget = m_target.h - m_pose.h + 2 * M_PI;
                    else if (m_target.h - m_pose.h > M_PI)
                        m_angleToTarget = m_target.h - m_pose.h - 2 * M_PI;
                    else
                        m_angleToTarget = m_target.h - m_pose.h;
                    turn(m_angleToTarget);
                    LOG_DEBUG("target destination reached, facing end move heading.");
                }

                //Change state
                m_distanceToTarget = 0;
                m_state = eNavState_TURNING_AT_TARGET;
            }
            break;
        }
    
        case eNavState_TURNING_AT_TARGET:
        {
            if (subOrderFinished())
            {
                m_angleToTarget = 0;
                m_state = eNavState_IDLE;
                m_order = eNavOrder_NOTHING;
                m_targetReached.set();
                LOG_INFO("order finished.");
            }
            break;
        }

        case eNavState_STOPPING:
        {
            if (subOrderFinished())
            {
                m_state = eNavState_IDLE;
                LOG_INFO("stopped.");
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
    stepperG.run();
    stepperD.run();
    //TODO compute_odom();
}

/**---------------------------------
 * User (= strategy) interface
 ---------------------------------*/

void Navigation::setPosition(PointCap newPose)
{
    //prevent any interrupt from occurring between any configuration of a left/right motor
    //portDISABLE_INTERRUPTS();
    m_pose = newPose.toAmbiPose(m_color);
    //portENABLE_INTERRUPTS();

    LOG_INFO("position set to :" + newPose.toString());
}

void Navigation::goTo(Point target, eDir sens)
{
    m_mutex.lock();

    if (m_state != eNavState_IDLE || m_order != eNavOrder_NOTHING )
    {
        LOG_INFO("new order pending until current order is finished");
        m_mutex.unlock();
        wait ();
        m_mutex.lock();
    }

    m_order = eNavOrder_GOTO;
    m_target = target.toAmbiPose (m_color);
    m_sensTarget = sens;

    m_mutex.unlock();
}

void Navigation::goToCap(PointCap target, eDir sens)
{
    m_mutex.lock();

    //If an order is present, wait
    if (m_state != eNavState_IDLE || m_order != eNavOrder_NOTHING )
    {
        LOG_INFO("new order pending until current order is finished");
        m_mutex.unlock();
        wait ();
        m_mutex.lock();
    }

    m_order = eNavOrder_GOTO_CAP;
    m_target = target.toAmbiPose (m_color);
    m_sensTarget = sens;

    m_mutex.unlock();
}

void Navigation::goForward(float distanceMm)
{
    ASSERT_TEXT(false, "not implemented");
    m_mutex.lock();

    //If an order is present, wait
    if (m_state != eNavState_IDLE || m_order != eNavOrder_NOTHING )
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
    if (m_state != eNavState_IDLE || m_order != eNavOrder_NOTHING )
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
    if (m_state != eNavState_IDLE || m_order != eNavOrder_NOTHING )
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
    //portDISABLE_INTERRUPTS();
    stepperG.stop();
    stepperD.stop();
    //portENABLE_INTERRUPTS();

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
    //portDISABLE_INTERRUPTS();
    if (s > 0)
    {
        stepperG.setMaxSpeed(s);
        stepperD.setMaxSpeed(s);
        m_speed = s;
    }
    else
    {
        stepperG.setMaxSpeed(SPEED_MAX);
        stepperD.setMaxSpeed(SPEED_MAX);
        m_speed = SPEED_MAX;
    }
    //portENABLE_INTERRUPTS();
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
        m_speed_virage = SPEED_MAX_VIR;
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
    //
    //WARNING : interrupt context !!
    //

    auto newStepG = stepperG.currentPosition();
    auto newStepD = stepperD.currentPosition();

    float dxG = newStepG - oldStepG;
    float dxD = newStepD - oldStepD;
    oldStepG = newStepG;
    oldStepD = newStepD;

    float ds = (dxG + dxD) / 2.;
    float dtheta = (dxD - dxG) / (2. * VOIE);

    m_pose.x += ds * 1; //TODO cos(m_pose.h + dtheta);
    m_pose.y += ds * 0; //TODO sin(m_pose.h + dtheta);
    m_pose.h = moduloPiPi(m_pose.h + dtheta);
}

void Navigation::straight(float mm)
{
    //prevent any interrupt from occurring between any configuration of a left/right motor
    //portDISABLE_INTERRUPTS();
    stepperG.setMaxSpeed(m_speed);
    stepperD.setMaxSpeed(m_speed);
    stepperG.move(-mm * GAIN_STEP_MM);
    stepperD.move(+mm * GAIN_STEP_MM);
    //portENABLE_INTERRUPTS();
}

void Navigation::turn(float angle)
{
    //prevent any interrupt from occurring between any configuration of a left/right motor
    //portDISABLE_INTERRUPTS();
    stepperG.setMaxSpeed(m_speed_virage);
    stepperD.setMaxSpeed(m_speed_virage);
    stepperG.move(-(-angle * VOIE / 2 * GAIN_STEP_MM));
    stepperD.move(+angle * VOIE / 2 * GAIN_STEP_MM);
    //portENABLE_INTERRUPTS();
}

void Navigation::interruptCurrentMove()
{
    LOG_INFO("current order is interrupted.");
    m_state = eNavState_STOPPING;
    //prevent any interrupt from occurring between any configuration of a left/right motor
    //portDISABLE_INTERRUPTS();
    stepperG.stop();
    stepperD.stop();
    //portENABLE_INTERRUPTS();
}

/**
 * return true if the trajectory is finished
 */
bool Navigation::subOrderFinished()
{
    //prevent any interrupt from occurring between any configuration of a left/right motor
    //portDISABLE_INTERRUPTS();
    bool res = stepperG.distanceToGo() == 0 || stepperD.distanceToGo() == 0;
    //portENABLE_INTERRUPTS();
    return res;
}

String Navigation::sensToString(eDir sens)
{
    switch (sens)
    {
    default:
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
    ENUM2STR(eNavState_IDLE)
;        ENUM2STR(eNavState_FACING_DEST);
        ENUM2STR(eNavState_GOING_TO_TARGET);
        ENUM2STR(eNavState_TURNING_AT_TARGET);
        ENUM2STR(eNavState_STOPPING);
    }
}
