/*
 * KinematicManager.cpp
 *
 *  Created on: 20 mai 2017
 *      Author: lambert.w
 */

#include "KinematicManager.h"
#include "Log.h"
#include "ArdMaths.h"

#define CHECK_RANGE(min, value, max) IS_OUT_RANGE(min, value, max) \
{ \
    LOG_ERROR(String(#value) + " is out of acceptable range [" + min + ", " +  max + "].");    \
}
#define CHECK_SPEED(speed) CHECK_RANGE(RobotParameters::MINBOUND_SPEED, speed, RobotParameters::MAXBOUND_SPEED);
using namespace ard;

KinematicManager::KinematicManager():
	m_mutex(),
	userMaxAcc(RobotParameters::MAXBOUND_ACC),
	userMaxTurnSpeed(RobotParameters::MAXBOUND_TURN_SPEED),
	userMaxTurnAcc(RobotParameters::MAXBOUND_TURN_ACC),
	userMaxSpeed(RobotParameters::MAXBOUND_SPEED),
	brakeMaxSpeedFront(0),
	brakeMaxSpeedRear(0),
	actuatorsMaxSpeedFront(0),
	actuatorsMaxSpeedRear(0),
	maxSpeedFront(0),
	maxSpeedRear(0),
	actuatorsFrontOut(false),
	actuatorsRearOut(false)
{
}

void KinematicManager::updateConf(RobotParameters* newConf)
{
	 RobotParametersListener::updateConf(newConf);

	 brakeMaxSpeedFront = sqrt(2*conf->maxAccCfg() * conf->avoidanceDistanceFront());
	 brakeMaxSpeedRear = sqrt(2*conf->maxAccCfg() * conf->avoidanceDistanceRear());
	 actuatorsMaxSpeedFront = sqrt(2 * conf->maxAccCfg() * (conf->avoidanceDistanceFront() - (conf->xavExtended() - conf->xav())));
	 actuatorsMaxSpeedRear = sqrt(2 * conf->maxAccCfg() * (conf->avoidanceDistanceRear() - (conf->xarExtended() - conf->xar())));

	 CHECK_SPEED(brakeMaxSpeedFront);
	 CHECK_SPEED(brakeMaxSpeedRear);
	 CHECK_SPEED(actuatorsMaxSpeedFront);
	 CHECK_SPEED(actuatorsMaxSpeedRear);

	 computeMaxSpeed();
}


void KinematicManager::frontActuatorsOut(bool out)
{
	if( out != actuatorsFrontOut)
	{
		m_mutex.lock();
		actuatorsFrontOut = out;
		computeMaxSpeed();
		m_mutex.unlock();
	}
}

void KinematicManager::rearActuatorsOut(bool out)
{
	if( out != actuatorsRearOut)
	{
		m_mutex.lock();
		actuatorsRearOut = out;
		computeMaxSpeed();
		m_mutex.unlock();
	}
}

void KinematicManager::setSpeedAcc(uint16_t vMax, uint16_t vMaxTurn, uint16_t accMax, uint16_t accMaxTurn)
{
    m_mutex.lock();

    //If a config is NULL, then restore default configuration
    if( vMax != 0 )
    {
        userMaxSpeed = saturate(vMax, RobotParameters::MINBOUND_SPEED, RobotParameters::MAXBOUND_SPEED);
        if( userMaxSpeed != vMax)
        	LOG_ERROR("New vMax config saturated");
    }
    else
        userMaxSpeed = RobotParameters::MAXBOUND_SPEED;


    if( vMaxTurn    != 0 )
    {
        userMaxTurnSpeed = saturate(vMaxTurn, RobotParameters::MINBOUND_TURN_SPEED, RobotParameters::MAXBOUND_TURN_SPEED);
        if(userMaxTurnSpeed < vMaxTurn)
        	LOG_ERROR("New vMaxTurn config saturated");
    }
    //If a config is NULL, then restore default configuration
    else
        userMaxTurnSpeed = RobotParameters::MAXBOUND_TURN_SPEED;


    if( accMax != 0 )
    {
        userMaxAcc = saturate(accMax, RobotParameters::MINBOUND_ACC, RobotParameters::MAXBOUND_ACC);
        if(userMaxAcc < accMax)
        	LOG_ERROR("New accMax config saturated");
    }
    //If a config is NULL, then restore default configuration
    else
        userMaxAcc = RobotParameters::MAXBOUND_ACC;


    if( accMaxTurn != 0 )
    {
        userMaxTurnAcc = saturate(accMaxTurn, RobotParameters::MINBOUND_TURN_ACC, RobotParameters::MAXBOUND_TURN_ACC);
        if(userMaxTurnAcc < accMaxTurn)
        	LOG_ERROR("New accMaxTurn config saturated");
    }
    //If a config is NULL, then restore default configuration
    else
        userMaxTurnAcc = RobotParameters::MAXBOUND_TURN_ACC;

    computeMaxSpeed();

    m_mutex.unlock();

    LOG_INFO(String("    user kinematic request: (") + userMaxSpeed + "mm/s, " + userMaxTurnSpeed +
            "°/s), acc set to (" + userMaxAcc + "mm/s², " + userMaxTurnAcc + "°/s²)");
}

void KinematicManager::computeMaxSpeed()
{
	//Find min speed among constraints in FRONT direction
	LinearSpeed old = maxSpeedFront;
	maxSpeedFront = conf->maxSpeedCfg();
	if( userMaxSpeed 			< maxSpeedFront ) 	maxSpeedFront = userMaxSpeed;
	if( brakeMaxSpeedFront 		< maxSpeedFront ) 	maxSpeedFront = brakeMaxSpeedFront;
	if( actuatorsFrontOut &&
	    actuatorsMaxSpeedFront  < maxSpeedFront ) 	maxSpeedFront = actuatorsMaxSpeedFront;

	if(old != maxSpeedFront)
	{
		if( maxSpeedFront == userMaxSpeed) LOG_INFO("Speed is limited by USER(strategy) request.");
		if( maxSpeedFront == brakeMaxSpeedFront) LOG_INFO("Speed is limited by FRONT BRAKE distance.");
		if( maxSpeedFront == actuatorsMaxSpeedFront) LOG_INFO("Speed is limited by FRONT ACTUATORS.");
	}

	//Find min speed among constraints in REAR direction
	old = maxSpeedRear;
	maxSpeedRear = conf->maxSpeedCfg();
	if( userMaxSpeed 			< maxSpeedRear ) 	maxSpeedRear  = userMaxSpeed;
	if( brakeMaxSpeedRear 		< maxSpeedRear ) 	maxSpeedRear  = brakeMaxSpeedRear;
	if( actuatorsRearOut &&
		actuatorsMaxSpeedRear 	< maxSpeedRear ) 	maxSpeedRear  = actuatorsMaxSpeedRear;

	if(old != maxSpeedRear)
	{
		if( maxSpeedRear == userMaxSpeed) LOG_INFO("Speed is limited by USER(strategy) request.");
		if( maxSpeedRear == brakeMaxSpeedRear) LOG_INFO("Speed is limited by BREAR RAKE distance.");
		if( maxSpeedRear == actuatorsMaxSpeedRear) LOG_INFO("Speed is limited by REAR ACTUATORS.");
	}
}

LinearSpeed KinematicManager::maxSpeed(eDir direction)
{
	m_mutex.lock();
	switch (direction) {
		case eDir_FORWARD:
			return maxSpeedFront;
			break;

		case eDir_BACKWARD:
			return maxSpeedRear;
			break;

		case eDir_BEST:
		default:
			return 0;
			break;
	}
	m_mutex.unlock();
}

LinearAcc KinematicManager::maxAcc()
{
	if(userMaxAcc < conf->maxAccCfg())
		return userMaxAcc;
	else
		return conf->maxAccCfg();
}

RotationSpeed KinematicManager::maxTurnSpeed()
{
	if(userMaxTurnSpeed < conf->maxTurnSpeedCfg())
		return userMaxTurnSpeed;
	else
		return conf->maxTurnSpeedCfg();
}

RotationAcc KinematicManager::maxTurnAcc()
{
	if(userMaxTurnAcc < conf->maxTurnAccCfg())
		return userMaxTurnAcc;
	else
		return conf->maxTurnAccCfg();
}
