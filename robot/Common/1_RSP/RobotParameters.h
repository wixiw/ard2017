/*
 * RobotParameters.h
 *
 *  Created on: 2 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_ROBOTPARAMETERS_H_
#define ROBOT_COMMON_1_RSP_ROBOTPARAMETERS_H_

#include <Com.h>
#include "ArdFramework.h"

#define GAIN_ANALOG_TENSION_BATTERIE 0.024

#define TABLE_BORDER_Y      1000
#define TABLE_BORDER_X      1500

#define SAFETY_AREA 80 //distance in mm from border where we ignore opponents

#define SERIAL_BAUDRATE 125000 //bit/s from 600 to 250k

#define LOG_QUEUE_SIZE 20       //number of logs that can be queued before being sent on the network

#define PEN_INSTALL_POSE 		Pose2D(  600,     800,   90)
#define PEN_START_POSE     		Pose2D(  760,   730+5,   180) //+5 to prevent to be out of start area, but not too much as we would hit the dispenser at rush
#define TRATION_INSTALL_POSE 	PEN_INSTALL_POSE
#define TRATION_START_POSE 		Pose2D(518-2,  785+20,  -90) //-2 to ensure room between 2 robots, +20 to leave room ahead in start area

//Actuators config
#define FUNNY_REST 			500
#define FUNNY_LAUNCH 		1000
#define ARM_MIN 			330 //was 295 but start position on flip flop is incorrect
#define ARM_MAX 			750
#define LIFTER_MIN 			390
#define LIFTER_MAX 			790

namespace ard
{
//forward declare
class RobotParameters;

class RobotParametersListener
{
public:
	RobotParametersListener();
	virtual ~RobotParametersListener() = default;

	//Reread the configuration and maps default config. Shall be called at least once
	//before the OS is initialized
	virtual void updateConf(RobotParameters* newConf);

	//Assert if not configured
	void ASSERT_CONFIGURED() const;

protected:
    RobotParameters* conf;
};

/**
 * This class contains all parameters of the robot model.
 * It contains the robot configuration which is part of the parameters.
 * It means that some parameters are not configured but computed from
 * other configuration data (typically : GAIN_XXX parameters).
 *
 * In order to have documentation on configurable parameters see RemoteControl.proto
 */
class RobotParameters
{
public:
    double GAIN_MM_2_STEPS_LEFT;   // gain to pass from a distance in mm to a number of steps on the left motor, *-1 because left motor is inverted
    double GAIN_MM_2_STEPS_RIGHT;  // gain to pass from a distance in mm to a number of steps on the right motor,
    double GAIN_STEPS_2_MM_LEFT;   // gain to convert a step count into a distance on left motor,
    double GAIN_STEPS_2_MM_RIGHT;  // gain to convert a step count into a distance on right motor,
    double GAIN_RAD_2_STEPS_LEFT;  // gain to convert an angle into a step count on left motor
    double GAIN_RAD_2_STEPS_RIGHT; // gain to convert an angle into a step count on left motor

    RobotParameters();

    //True if conf is updated successfully, false : unchanged
    bool setConfig(apb_Configuration const& newConf);
    apb_Configuration const& getConfig() const {return cfg;};

    uint32_t stepByTurn()           	const;
    Distance xav()                  	const;
    Distance xar()                  	const;
    Distance yside()                	const;
    Distance xavExtended()          	const;
    Distance xarExtended()          	const;
    Distance xouter()               	const;
    float leftWheelDiameter()    		const;
    float rightWheelDiameter()  		const;
    float voie()                 		const;
    LinearAcc maxAccCfg()              	const;
    LinearSpeed maxSpeedCfg()  			const;
    LinearSpeed recalSpeed()        	const;
    RotationAcc maxTurnAccCfg()        	const;
    RotationSpeed maxTurnSpeedCfg()    	const;
    Distance avoidanceDistanceFront()   const;
    Distance avoidanceDistanceRear()    const;
    DelayMs strategyDuration() 	     	const;
    DelayMs detectionWaitForOppMove()	const;
    bool detectionActive()      		const;

    char const* const serialNumber() 	const;
    bool logDebug()                 	const;
    bool bipAllowed()               	const;

    static const LinearSpeed MINBOUND_SPEED          = 50;   //mm/s
    static const LinearSpeed MAXBOUND_SPEED          = 2000; //mm/s
    static const LinearAcc MINBOUND_ACC              = 300;  //mm/s²
    static const LinearAcc MAXBOUND_ACC              = 4000; //mm/s²
    static const RotationSpeed MINBOUND_TURN_SPEED   = 30;   //°/s
    static const RotationSpeed MAXBOUND_TURN_SPEED   = 800;  //°/s
    static const RotationAcc MINBOUND_TURN_ACC       = 100;  //°/s²
    static const RotationAcc MAXBOUND_TURN_ACC       = 2000; //°/s²

private:
    apb_Configuration cfg;

    bool m_configuredOnce;

    void setComputedVars();
    bool checkConfig(apb_Configuration const& newConf);
};

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_ROBOTPARAMETERS_H_ */
