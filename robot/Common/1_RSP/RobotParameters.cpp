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

RobotParameters::RobotParameters():
	GAIN_MM_2_STEPS_LEFT(0.0),
	GAIN_MM_2_STEPS_RIGHT(0.0),
	GAIN_STEPS_2_MM_LEFT(0.0),
	GAIN_STEPS_2_MM_RIGHT(0.0),
	GAIN_RAD_2_STEPS_LEFT(0.0),
	GAIN_RAD_2_STEPS_RIGHT(0.0),
	cfg(),
	m_maxSpeedFront(0),
	m_maxSpeedRear(0),
	m_configuredOnce(false),
	m_mutex(),
	userMaxSpeed(0),
	userMaxTurnSpeed(0),
	userMaxAcc(0),
	userMaxTurnAcc(0)
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
    cfg.maxAcc                  = 1300;
    cfg.maxTurnAcc              = 400;
    cfg.recalSpeed              = 210.0;
    cfg.maxTurnSpeed            = 500.0;
    cfg.deccDist                = 100.0;
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
    m_maxSpeedFront = sqrt(2 * cfg.maxAcc * cfg.deccDist);                           //Typ : dec = 700, dist = 150 => 450mm
    m_maxSpeedRear = m_maxSpeedFront;                           //Typ : dec = 700, dist = 150 => 450mm

    userMaxSpeed        = RobotParameters::MAXBOUND_MAXSPEED;
    userMaxTurnSpeed    = RobotParameters::MAXBOUND_MAXTURNSPEED;
    userMaxAcc          = RobotParameters::MAXBOUND_MAXACC;
    userMaxTurnAcc      = RobotParameters::MAXBOUND_MAXTURNACC;
}

#define CHECK_RANGE(min, value, max) IS_OUT_RANGE(min, value, max) \
{ \
    LOG_ERROR(String(#value) + " is out of acceptable range [" + min + ", " +  max + "].");    \
    res &= false;   \
}

bool RobotParameters::checkConfig(apb_Configuration const& newConf)
{
    bool res = true;
    double newMaxSpeed = sqrt(2 * newConf.maxAcc * newConf.deccDist);

    //Checks :
    CHECK_RANGE(5, newConf.xav, 500);
    CHECK_RANGE(5, newConf.xar, 500);
    CHECK_RANGE(5, newConf.yside, 500);
    CHECK_RANGE(newConf.xav, newConf.xavExtended, 500);
    CHECK_RANGE(50, newConf.leftWheelDiameter, 70);
    CHECK_RANGE(50, newConf.rightWheelDiameter, 70);
    CHECK_RANGE(50, newConf.voie, 300);
    CHECK_RANGE(MINBOUND_MAXACC, newConf.maxAcc, MAXBOUND_MAXACC);
    CHECK_RANGE(MINBOUND_MAXTURNSPEED, newConf.maxTurnSpeed, MAXBOUND_MAXTURNSPEED);
    CHECK_RANGE(50, newConf.recalSpeed, 500);
    CHECK_RANGE(MINBOUND_MAXTURNACC, newConf.maxTurnAcc, MAXBOUND_MAXTURNACC);
    CHECK_RANGE(20, newConf.deccDist, 500);
    if (newConf.strategyDuration)
    {
        CHECK_RANGE(50000, newConf.strategyDuration, 89999);
    }
    CHECK_RANGE(50, newConf.detectionWaitForOppMove, 5000);
    CHECK_RANGE(MINBOUND_MAXSPEED, newMaxSpeed, MAXBOUND_MAXSPEED);

    if(strcmp(newConf.serialNumber, "") == 0)
    {
        LOG_ERROR("Serial number shall not be empty");
        res = false;
    }

    if( newConf.deccDist < newConf.xavExtended - newConf.xav)
    {
        LOG_ERROR("Decceleration distance is too low for front actuators extension");
        res = false;
    }

    if( newConf.deccDist < newConf.xarExtended - newConf.xar)
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

int32_t RobotParameters::xav() const
{
    ASSERT(m_configuredOnce);
    return cfg.xav;
}

int32_t RobotParameters::xar() const
{
    ASSERT(m_configuredOnce);
    return cfg.xar;
}

int32_t RobotParameters::yside() const
{
    ASSERT(m_configuredOnce);
    return cfg.yside;
}

int32_t RobotParameters::xavExtended() const
{
    ASSERT(m_configuredOnce);
    return cfg.xavExtended;
}

int32_t ard::RobotParameters::xarExtended() const {
    ASSERT(m_configuredOnce);
    return cfg.xarExtended;
}

int32_t RobotParameters::xouter() const
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

uint32_t RobotParameters::maxAcc() const
{
    ASSERT(m_configuredOnce);
    return min(userMaxAcc, cfg.maxAcc);
}

uint32_t RobotParameters::maxTurnAcc() const
{
    ASSERT(m_configuredOnce);
    return min(userMaxTurnAcc, cfg.maxTurnAcc);
}

uint32_t RobotParameters::maxTurnSpeed() const
{
    ASSERT(m_configuredOnce);
    return min(userMaxTurnSpeed, cfg.maxTurnSpeed);
}

int32_t RobotParameters::deccDist() const
{
    ASSERT(m_configuredOnce);
    return cfg.deccDist;
}

uint32_t RobotParameters::strategyDuration() const
{
    ASSERT(m_configuredOnce);
    return cfg.strategyDuration;
}

uint32_t RobotParameters::detectionWaitForOppMove() const
{
    ASSERT(m_configuredOnce);
    return cfg.detectionWaitForOppMove;
}

bool RobotParameters::detectionActive() const
{
    ASSERT(m_configuredOnce);
    return cfg.detectionActive;
}

uint32_t RobotParameters::recalSpeed() const
{
    ASSERT(m_configuredOnce);
    return cfg.recalSpeed;
}

uint32_t RobotParameters::maxSpeed(eDir sens) const
{
    ASSERT(m_configuredOnce);
    switch (sens) {
		case eDir_FORWARD:
			return m_maxSpeedFront;
			break;

		case eDir_BACKWARD:
			return m_maxSpeedRear;
			break;

		case eDir_BEST:
		default:
			ASSERT(false);
			return 0;
			break;
	}
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

void RobotParameters::frontActuatorsOut(bool out)
{
	m_mutex.lock();
	m_maxSpeedFront = min(userMaxSpeed, sqrt(2 * cfg.maxAcc * (cfg.deccDist - out*(cfg.xavExtended - cfg.xav))));
	m_mutex.unlock();
//TODO
	//	if(out)
//		LOG_INFO("Max speed (front) limitated due to actuators");
//	else
//		LOG_INFO("Max speed (front) no more limited due to actuators");
}

void RobotParameters::rearActuatorsOut(bool out)
{
	m_mutex.lock();
	m_maxSpeedRear = min(userMaxSpeed, sqrt(2 * cfg.maxAcc * (cfg.deccDist - out*(cfg.xarExtended - cfg.xar))));
	m_mutex.unlock();
	//TODO
//	if(out)
//		LOG_INFO("Max speed (rear) limitated due to actuators");
//	else
//		LOG_INFO("Max speed (rear) no more limited due to actuators");
}

void RobotParameters::setSpeedAcc(uint16_t vMax, uint16_t vMaxTurn, uint16_t accMax, uint16_t accMaxTurn)
{
	NOT_IMPLEMENTED();
    m_mutex.lock();

    //If a config is NULL, then restore default configuration
    if( vMax != 0 )
    {
        userMaxSpeed = saturate(vMax, RobotParameters::MINBOUND_MAXSPEED, RobotParameters::MAXBOUND_MAXSPEED);
        if( userMaxSpeed != vMax)
        	LOG_ERROR("New vMax config saturated");
    }
    else
        userMaxSpeed = sqrt(2 * cfg.maxAcc * cfg.deccDist);

    //If a config is NULL, then restore default configuration
    if( vMaxTurn    != 0 )
    {
        userMaxTurnSpeed = saturate(vMaxTurn, RobotParameters::MINBOUND_MAXTURNSPEED, RobotParameters::MAXBOUND_MAXTURNSPEED);
        if(userMaxTurnSpeed < vMaxTurn)
        	LOG_ERROR("New vMaxTurn config saturated");
    }
    else
        userMaxTurnSpeed = cfg.maxTurnSpeed;

    //If a config is NULL, then restore default configuration
    if( accMax != 0 )
    {
        userMaxAcc = saturate(accMax, RobotParameters::MINBOUND_MAXACC, RobotParameters::MAXBOUND_MAXACC);
        if(userMaxAcc < accMax)
        	LOG_ERROR("New accMax config saturated");
    }
    else
        userMaxAcc = cfg.maxAcc;

    //If a config is NULL, then restore default configuration
    if( accMaxTurn != 0 )
    {
        userMaxTurnAcc = saturate(accMaxTurn, RobotParameters::MINBOUND_MAXTURNACC, RobotParameters::MAXBOUND_MAXTURNACC);
        if(userMaxTurnAcc < accMaxTurn)
        	LOG_ERROR("New accMaxTurn config saturated");
    }
    else
        userMaxTurnAcc = cfg.maxTurnAcc;

    m_mutex.unlock();

    LOG_INFO(String("    speed set to : (") + userMaxSpeed + "mm/s, " + userMaxTurnSpeed +
            "°/s), acc set to (" + userMaxAcc + "mm/s², " + userMaxTurnAcc + "°/s²)");
}
