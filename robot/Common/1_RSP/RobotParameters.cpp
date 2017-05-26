/*
 * RobotConfig.cpp
 *
 *  Created on: 2 avr. 2017
 *      Author: wix
 */

#include "RobotParameters.h"
#include "core/ArdMaths.h"
#include "Log.h"

using namespace ard;

RobotParametersListener::RobotParametersListener():
		conf(NULL)
{

}

void RobotParametersListener::updateConf(RobotParameters* newConf)
{
    ASSERT(newConf);
    conf = newConf;
}

void RobotParametersListener::ASSERT_CONFIGURED() const
{
	ASSERT_TEXT(conf, "You forgot to link a config in a param listener");
}

/**
 * -----------------------------------------------------
 */

RobotParameters::RobotParameters():
	GAIN_MM_2_STEPS_LEFT(0.0),
	GAIN_MM_2_STEPS_RIGHT(0.0),
	GAIN_STEPS_2_MM_LEFT(0.0),
	GAIN_STEPS_2_MM_RIGHT(0.0),
	GAIN_RAD_2_STEPS_LEFT(0.0),
	GAIN_RAD_2_STEPS_RIGHT(0.0),
	cfg(),
	m_configuredOnce(false)
{
    strcpy(cfg.serialNumber,    "");
    cfg.stepByTurn              = 1600;
    cfg.xav                     = 145;//mm
    cfg.xar                     = 30;//mm
    cfg.yside                   = 88;//mm
    cfg.xavExtended             = 210;//mm
    cfg.xarExtended             = cfg.xar;//mm
    cfg.xouter                  = 170;//mm
    cfg.leftWheelDiameter       = 60.000;//mm
    cfg.rightWheelDiameter      = 60.000;//mm
    cfg.voie                    = 160.4;//mm
    cfg.maxAcc                  = 500;
    cfg.maxSpeed                = 250;
    cfg.recalSpeed              = 180;
    cfg.maxTurnAcc              = 150;
    cfg.maxTurnSpeed            = 150;
    cfg.avoidanceDistFront      = 130;
    cfg.avoidanceDistRear       = 130;
    cfg.strategyDuration        = 89500;
    cfg.detectionWaitForOppMove = 1000;
    cfg.detectionActive         = true;
    cfg.logDebug                = false;
    cfg.bipAllowed              = true;
}

void RobotParameters::setComputedVars()
{
    GAIN_MM_2_STEPS_LEFT = -(cfg.stepByTurn / (cfg.leftWheelDiameter * M_PI));  //Typ : 8.50
    GAIN_MM_2_STEPS_RIGHT = cfg.stepByTurn / (cfg.rightWheelDiameter * M_PI);
    GAIN_STEPS_2_MM_LEFT = 1 / GAIN_MM_2_STEPS_LEFT;
    GAIN_STEPS_2_MM_RIGHT = 1 / GAIN_MM_2_STEPS_RIGHT;
    GAIN_RAD_2_STEPS_LEFT = cfg.voie * GAIN_MM_2_STEPS_LEFT / 2.;               //Typ : 680
    GAIN_RAD_2_STEPS_RIGHT = cfg.voie * GAIN_MM_2_STEPS_RIGHT / 2.;
}

#define CHECK_RANGE(min, value, max) IS_OUT_RANGE(min, value, max) \
{ \
    LOG_ERROR(String(#value) + " is out of acceptable range [" + min + ", " +  max + "].");    \
    res &= false;   \
}

bool RobotParameters::checkConfig(apb_Configuration const& newConf)
{
    bool res = true;

    //Checks :
    CHECK_RANGE(5, newConf.xav, 500);
    CHECK_RANGE(5, newConf.xar, 500);
    CHECK_RANGE(5, newConf.yside, 500);
    CHECK_RANGE(newConf.xav, newConf.xavExtended, 500);
    CHECK_RANGE(50, newConf.leftWheelDiameter, 70);
    CHECK_RANGE(50, newConf.rightWheelDiameter, 70);
    CHECK_RANGE(50, newConf.voie, 300);
    CHECK_RANGE(MINBOUND_ACC, newConf.maxAcc, MAXBOUND_ACC);
    CHECK_RANGE(MINBOUND_SPEED, newConf.maxSpeed, MAXBOUND_SPEED);
    CHECK_RANGE(50, newConf.recalSpeed, 500);
    CHECK_RANGE(MINBOUND_TURN_SPEED, newConf.maxTurnSpeed, MAXBOUND_TURN_SPEED);
    CHECK_RANGE(MINBOUND_TURN_ACC, newConf.maxTurnAcc, MAXBOUND_TURN_ACC);
    CHECK_RANGE(50, newConf.avoidanceDistFront, 500);
    CHECK_RANGE(50, newConf.avoidanceDistRear, 500);
    if (newConf.strategyDuration)
    {
        CHECK_RANGE(50000, newConf.strategyDuration, 89999);
    }
    CHECK_RANGE(50, newConf.detectionWaitForOppMove, 5000);

    if(strcmp(newConf.serialNumber, "") == 0)
    {
        LOG_ERROR("Serial number shall not be empty");
        res = false;
    }

    if( newConf.avoidanceDistFront < newConf.xavExtended - newConf.xav)
    {
        LOG_ERROR("Decceleration distance is too low for front actuators extension");
        res = false;
    }

    if( newConf.avoidanceDistRear < newConf.xarExtended - newConf.xar)
    {
        LOG_ERROR("Decceleration distance is too low for rear actuators extension");
        res = false;
    }

    return res;
}

bool RobotParameters::setConfig(apb_Configuration const& newConf)
{
	bool res = false;

    //update config.
    if (checkConfig(newConf))
    {
        cfg = newConf;
        setComputedVars();
        if( m_configuredOnce )
            LOG_INFO("New configuration applied");
        else
            m_configuredOnce = true;

        res = true;
    }
    else
    {
        LOG_ERROR("Configuration not applied");
        res = false;
    }

    return res;
}

uint32_t RobotParameters::stepByTurn() const
{
    ASSERT(m_configuredOnce);
    return cfg.stepByTurn;
}

Distance RobotParameters::xav() const
{
    ASSERT(m_configuredOnce);
    return cfg.xav;
}

Distance RobotParameters::xar() const
{
    ASSERT(m_configuredOnce);
    return cfg.xar;
}

Distance RobotParameters::yside() const
{
    ASSERT(m_configuredOnce);
    return cfg.yside;
}

Distance RobotParameters::xavExtended() const
{
    ASSERT(m_configuredOnce);
    return cfg.xavExtended;
}

Distance ard::RobotParameters::xarExtended() const {
    ASSERT(m_configuredOnce);
    return cfg.xarExtended;
}

Distance RobotParameters::xouter() const
{
    ASSERT(m_configuredOnce);
    return cfg.xouter;
}

float RobotParameters::leftWheelDiameter() const
{
    ASSERT(m_configuredOnce);
    return cfg.leftWheelDiameter;
}

float RobotParameters::rightWheelDiameter() const
{
    ASSERT(m_configuredOnce);
    return cfg.rightWheelDiameter;
}

float RobotParameters::voie() const
{
    ASSERT(m_configuredOnce);
    return cfg.voie;
}

LinearAcc RobotParameters::maxAccCfg() const
{
    ASSERT(m_configuredOnce);
    return cfg.maxAcc;
}

LinearSpeed RobotParameters::maxSpeedCfg() const
{
    ASSERT(m_configuredOnce);
    return cfg.maxSpeed;
}

LinearSpeed RobotParameters::recalSpeed() const
{
    ASSERT(m_configuredOnce);
    return cfg.recalSpeed;
}

RotationAcc RobotParameters::maxTurnAccCfg() const
{
    ASSERT(m_configuredOnce);
    return cfg.maxTurnAcc;
}

RotationSpeed RobotParameters::maxTurnSpeedCfg() const
{
    ASSERT(m_configuredOnce);
    return cfg.maxTurnSpeed;
}

Distance RobotParameters::avoidanceDistanceFront() const
{
    ASSERT(m_configuredOnce);
    return cfg.avoidanceDistFront;
}

Distance RobotParameters::avoidanceDistanceRear() const
{
    ASSERT(m_configuredOnce);
    return cfg.avoidanceDistRear;
}

DelayMs RobotParameters::strategyDuration() const
{
    ASSERT(m_configuredOnce);
    return cfg.strategyDuration;
}

DelayMs RobotParameters::detectionWaitForOppMove() const
{
    ASSERT(m_configuredOnce);
    return cfg.detectionWaitForOppMove;
}

bool RobotParameters::detectionActive() const
{
    ASSERT(m_configuredOnce);
    return cfg.detectionActive;
}

char const* const RobotParameters::serialNumber() const
{
    ASSERT(m_configuredOnce);
    return cfg.serialNumber;
}

bool RobotParameters::logDebug() const
{
    ASSERT(m_configuredOnce);
    return cfg.logDebug;
}

bool RobotParameters::bipAllowed() const
{
    ASSERT(m_configuredOnce);
    return cfg.bipAllowed;
}


