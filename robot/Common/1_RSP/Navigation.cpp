#include <Arduino.h>
#include "LogThread.h"
#include "Navigation.h"
#include "K_constants.h"
#include "BSP.h"

using namespace ard;

Navigation::Navigation()
        :
                m_pose(),
                m_state(eNavState::IDLE),
                m_target(),
                m_sensTarget(eDir_UNDEFINED),
                m_order(eNavOrder::NOTHING),
                m_angleToTarget(0),
                m_distanceToTarget(0),
                stepperG(AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR),
                stepperD(AccelStepper::DRIVER, PAPD_STEP, PAPD_DIR),
                omronFrontLeft(OMRON1, 50, 50),
                omronFrontRight(OMRON2, 50, 50),
                omronRearLeft(OMRON3, 50, 50),
                omronRearRight(OMRON4, 50, 50),
                m_color(eColor_PREF),
                m_speed(SPEED_MAX),
                m_speed_virage(SPEED_MAX_VIR),
                m_mutex(NULL),
                m_targetReached(NULL),
                oldStepG(0),
                oldStepD(0)
{
}

/**---------------------------------
 * Container thread interface
 ---------------------------------*/

void Navigation::init()
{
    m_mutex = g_ArdOs.Mutex_create();
    m_targetReached = g_ArdOs.Signal_create();
    stepperG.setAcceleration(ACC_MAX);
    stepperD.setAcceleration(ACC_MAX);
}

void Navigation::update(TimeMs sinceLastCall)
{
    static auto lastState = m_state;
    if (m_state != lastState)
    {
        LOG_DEBUG("NAV : state changed from  " + stateToString(lastState) + " to " + stateToString(m_state));
        lastState = m_state;
    }

    //Take a mutex to prevent localisation and target to be changed during a cycle
    g_ArdOs.Mutex_lock(m_mutex);

    switch (m_state)
    {
    default:
    case eNavState::IDLE:
    {
        switch (m_order)
        {
        default:
        case eNavOrder::NOTHING:
        {
            break;
        }

        case eNavOrder::GOTO:
        case eNavOrder::GOTO_CAP:
        {
            LOG_INFO(
                    "NAV : new order " + orderToString(m_order) + "(" + m_target.x + ", " + m_target.y + ", " + m_target.h + ") " + sensToString(m_sensTarget)
                            + ".");
            turn(m_angleToTarget);
            m_state = eNavState::FACING_DEST;
            break;
        }
        }
        break;
    }

    case eNavState::FACING_DEST:
    {
        if (subOrderFinished())
        {
            //Request straight line
            m_distanceToTarget = m_sensTarget * m_pose.distanceTo(m_target);
            straight(m_distanceToTarget);

            //Change state
            m_angleToTarget = 0;
            m_state = eNavState::GOING_TO_TARGET;
            LOG_DEBUG("NAV : facing destination, beginning line.");
        }
        break;
    }

    case eNavState::GOING_TO_TARGET:
    {
        if (subOrderFinished())
        {
            //Request rotation to final heading
            if (m_order == eNavOrder::GOTO_CAP)
            {
                if (m_target.h - m_pose.h < -M_PI)
                    m_angleToTarget = m_target.h - m_pose.h + 2 * M_PI;
                else if (m_target.h - m_pose.h > M_PI)
                    m_angleToTarget = m_target.h - m_pose.h - 2 * M_PI;
                else
                    m_angleToTarget = m_target.h - m_pose.h;
                turn(m_angleToTarget);
            }

            //Change state
            m_distanceToTarget = 0;
            m_state = eNavState::TURNING_AT_TARGET;
        }
        break;
    }

    case eNavState::TURNING_AT_TARGET:
    {
        if (subOrderFinished())
        {
            m_angleToTarget = 0;
            m_state = eNavState::IDLE;
            m_order = eNavOrder::NOTHING;
            g_ArdOs.Signal_set(m_targetReached);
            LOG_INFO("NAV : order finished.");
        }
        break;
    }

    case eNavState::STOPPING:
    {
        if (subOrderFinished())
        {
            m_state = eNavState::IDLE;
            LOG_INFO("NAV : stopped.");
        }
        break;
    }
    }

    g_ArdOs.Mutex_unlock(m_mutex);
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
    //TODO portDISABLE_INTERRUPTS();
    m_pose = newPose.toAmbiPose(m_color);
    //TODO portENABLE_INTERRUPTS();

    LOG_INFO("NAV : position set to :" + newPose.toString());
}

void Navigation::goTo(Point target, eDir sens)
{
    g_ArdOs.Mutex_lock(m_mutex);
    //If an order is present, wait
    if (m_state != eNavState::IDLE)
    {
        LOG_INFO("NAV : new order pending until current order is finished");
        g_ArdOs.Mutex_unlock(m_mutex);
        wait ();
        g_ArdOs.Mutex_lock(m_mutex);
    }

    m_order = eNavOrder::GOTO;
    m_target = target.toAmbiPose (m_color);
    m_sensTarget = sens;

    g_ArdOs.Mutex_unlock(m_mutex);
}

void Navigation::goToCap(PointCap target, eDir sens)
{
    g_ArdOs.Mutex_lock(m_mutex);

    //If an order is present, wait
    if (m_state != eNavState::IDLE)
    {
        LOG_INFO("NAV : new order pending until current order is finished");
        g_ArdOs.Mutex_unlock(m_mutex);
        wait ();
        g_ArdOs.Mutex_lock(m_mutex);
    }

    m_order = eNavOrder::GOTO_CAP;
    m_target = target.toAmbiPose (m_color);
    m_sensTarget = sens;

    g_ArdOs.Mutex_unlock(m_mutex);
}

void Navigation::goForward(float distanceMm)
{
    ardAssert(false, "not implemented");
    g_ArdOs.Mutex_lock(m_mutex);

    //If an order is present, wait
    if (m_state != eNavState::IDLE)
    {
        LOG_INFO("NAV : new order pending until current order is finished");
        g_ArdOs.Mutex_unlock(m_mutex);
        wait();
        g_ArdOs.Mutex_lock(m_mutex);
    }

    //TODO
//  m_order = XXX;
//  m_target = target.toAmbiPose(m_color);
//  m_sensTarget = sens;

    g_ArdOs.Mutex_unlock(m_mutex);
}

void Navigation::turnTo(float angle)
{
    ardAssert(false, "not implemented");
    g_ArdOs.Mutex_lock(m_mutex);

    //If an order is present, wait
    if (m_state != eNavState::IDLE)
    {
        LOG_INFO("NAV : new order pending until current order is finished");
        g_ArdOs.Mutex_unlock(m_mutex);
        wait();
        g_ArdOs.Mutex_lock(m_mutex);
    }

    //TODO
//  m_order = XXX;
//  m_target = target.toAmbiPose(m_color);
//  m_sensTarget = sens;

    g_ArdOs.Mutex_unlock(m_mutex);
}

void Navigation::faceTo(Point p)
{
    ardAssert(false, "not implemented");
    g_ArdOs.Mutex_lock(m_mutex);

    //If an order is present, wait
    if (m_state != eNavState::IDLE)
    {
        LOG_INFO("NAV : new order pending until current order is finished");
        g_ArdOs.Mutex_unlock(m_mutex);
        wait();
        g_ArdOs.Mutex_lock(m_mutex);
    }

    //TODO
//  m_order = XXX;
//  m_target = target.toAmbiPose(m_color);
//  m_sensTarget = sens;

    g_ArdOs.Mutex_unlock(m_mutex);
}

void Navigation::stop()
{
    LOG_INFO("NAV : stop requested");
    g_ArdOs.Mutex_lock(m_mutex);

    //prevent any interrupt from occurring between any configuration of a left/right motor
    //TODO portDISABLE_INTERRUPTS();
    stepperG.stop();
    stepperD.stop();
    //TODO portENABLE_INTERRUPTS();

    //The state is directly changed to interrupting
    m_state = eNavState::STOPPING;
    m_order = Navigation::eNavOrder::NOTHING;

    g_ArdOs.Mutex_unlock(m_mutex);
}

void Navigation::wait()
{
    //obviously don't put a mutex, it's a blocking call ... !
    g_ArdOs.Signal_wait(m_targetReached);
}

bool Navigation::targetReached()
{
    g_ArdOs.Mutex_lock(m_mutex);
    bool res = m_state == eNavState::IDLE;
    g_ArdOs.Mutex_unlock(m_mutex);
    return res;
}

/**---------------------------------
 * Nav configuration
 ---------------------------------*/

void Navigation::setColor(eColor c)
{
    ardAssert(c != eColor_UNKNOWN, "NAV : color should not be set to undefined.");
    g_ArdOs.Mutex_lock(m_mutex);
    m_color = c;
    g_ArdOs.Mutex_unlock(m_mutex);
}

void Navigation::setSpeed(float s)
{
    //prevent any interrupt from occurring between any configuration of a left/right motor
    //TODO portDISABLE_INTERRUPTS();
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
    //TODO portENABLE_INTERRUPTS();
}

void Navigation::setSpeedVir(float s)
{
    g_ArdOs.Mutex_lock(m_mutex);
    if (s > 0)
    {
        m_speed_virage = s;
    }
    else
    {
        m_speed_virage = SPEED_MAX_VIR;
    }
    g_ArdOs.Mutex_unlock(m_mutex);
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

    m_pose.x += ds * cos(m_pose.h + dtheta);
    m_pose.y += ds * sin(m_pose.h + dtheta);
    m_pose.h = moduloPiPi(m_pose.h + dtheta);
}

void Navigation::straight(float mm)
{
    //prevent any interrupt from occurring between any configuration of a left/right motor
    //TODO portDISABLE_INTERRUPTS();
    stepperG.setMaxSpeed(m_speed);
    stepperD.setMaxSpeed(m_speed);
    stepperG.move(-mm * GAIN_STEP_MM);
    stepperD.move(+mm * GAIN_STEP_MM);
    //TODO portENABLE_INTERRUPTS();
}

void Navigation::turn(float angle)
{
    //prevent any interrupt from occurring between any configuration of a left/right motor
    //TODO portDISABLE_INTERRUPTS();
    stepperG.setMaxSpeed(m_speed_virage);
    stepperD.setMaxSpeed(m_speed_virage);
    stepperG.move(-(-angle * VOIE / 2 * GAIN_STEP_MM));
    stepperD.move(+angle * VOIE / 2 * GAIN_STEP_MM);
    //TODO portENABLE_INTERRUPTS();
}

void Navigation::interruptCurrentMove()
{
    LOG_INFO("NAV : current order is interrupted.");
    m_state = eNavState::STOPPING;
    //prevent any interrupt from occurring between any configuration of a left/right motor
    //TODO portDISABLE_INTERRUPTS();
    stepperG.stop();
    stepperD.stop();
    //TODO portENABLE_INTERRUPTS();
}

/**
 * return true if the trajectory is finished
 */
bool Navigation::subOrderFinished()
{
    //prevent any interrupt from occurring between any configuration of a left/right motor
    //TODO portDISABLE_INTERRUPTS();
    bool res = stepperG.distanceToGo() == 0 || stepperD.distanceToGo() == 0;
    //TODO portENABLE_INTERRUPTS();
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
    ENUM2STR(eNavOrder::NOTHING)
;        ENUM2STR(eNavOrder::GOTO);
        ENUM2STR(eNavOrder::GOTO_CAP);
    }
}

String Navigation::stateToString(eNavState state)
{
    switch (state)
    {
    default:
    ENUM2STR(eNavState::IDLE)
;        ENUM2STR(eNavState::FACING_DEST);
        ENUM2STR(eNavState::GOING_TO_TARGET);
        ENUM2STR(eNavState::TURNING_AT_TARGET);
        ENUM2STR(eNavState::STOPPING);
    }
}
