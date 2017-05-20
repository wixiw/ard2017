/*
 * KinematicManager.h
 *
 *  Created on: 20 mai 2017
 *      Author: lambert.w
 */

#ifndef ROBOT_COMMON_1_RSP_MOTION_KINEMATICMANAGER_H_
#define ROBOT_COMMON_1_RSP_MOTION_KINEMATICMANAGER_H_

#include "ArdOs.h"
#include "RobotParameters.h"

namespace ard
{
	/**
	 * This class manage robot max speed and max acc depending on different configuration sources/constraints
	 */
	class KinematicManager: public RobotParametersListener
	{
	public:
		KinematicManager();

		//override RobotParametersListener
		//no mutex protection as it's not expected to happen during a match
		//either init() or vizy cmd
		virtual void updateConf(RobotParameters* newConf) override;

	    //inform that the front robot actuators are out (or not) => recompute maxSpeed
	    void frontActuatorsOut(bool out);

	    //inform that the rear robot actuators are out (or not) => recompute maxSpeed
	    void rearActuatorsOut(bool out);

	    //Set a new user speed/acc constraint
	    void setSpeedAcc(uint16_t vMax /*mm/s*/, uint16_t vMaxsTurn /*°/s*/,
	                    uint16_t accMax /*mm/s2*/, uint16_t accMaxTurn /*°/s2*/);

	    //Accessors (not const due to the mutex)
	    LinearSpeed 	maxSpeed(eDir direction);
	    LinearAcc 		maxAcc();
	    RotationSpeed 	maxTurnSpeed();
	    RotationAcc 	maxTurnAcc();

	protected:
	    FakeMutex m_mutex;

	    //move constraints
	    LinearAcc 		userMaxAcc;        //mm/s²
	    RotationSpeed 	userMaxTurnSpeed;  //°/s
	    RotationAcc 	userMaxTurnAcc;    //°/s²

	    //Maximal linear speed
	    LinearSpeed userMaxSpeed;      //mm/s
	    LinearSpeed brakeMaxSpeedFront;  //in mm/s : maximal speed to brake within avoidance in front direction
	    LinearSpeed brakeMaxSpeedRear;  //in mm/s : maximal speed to brake within avoidance in rear direction
	    LinearSpeed actuatorsMaxSpeedFront;
	    LinearSpeed actuatorsMaxSpeedRear;
	    LinearSpeed maxSpeedFront;		//computed spped in front directioj=n
	    LinearSpeed maxSpeedRear;       //computed spped in front directioj=n

	    //Cache to help logs
	    bool actuatorsFrontOut;
	    bool actuatorsRearOut;

	    void computeMaxSpeed();
	};

}

#endif /* ROBOT_COMMON_1_RSP_MOTION_KINEMATICMANAGER_H_ */
