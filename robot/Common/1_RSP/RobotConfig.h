/*
 * RobotConfig.h
 *
 *  Created on: 2 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_ROBOTCONFIG_H_
#define ROBOT_COMMON_1_RSP_ROBOTCONFIG_H_

#include <Com.h>

namespace ard
{

class RobotConfig
{
public:
    double GAIN_MM_2_STEPS_LEFT;   // gain to pass from a distance in mm to a number of steps on the left motor, *-1 because left motor is inverted
    double GAIN_MM_2_STEPS_RIGHT;  // gain to pass from a distance in mm to a number of steps on the right motor,
    double GAIN_STEPS_2_MM_LEFT;   // gain to convert a step count into a distance on left motor,
    double GAIN_STEPS_2_MM_RIGHT;  // gain to convert a step count into a distance on right motor,
    double GAIN_RAD_2_STEPS_LEFT;  // gain to convert an angle into a step count on left motor
    double GAIN_RAD_2_STEPS_RIGHT; // gain to convert an angle into a step count on left motor

    RobotConfig();

    void updateConfig(apb_Configuration const& newConf);
    apb_Configuration const& copy() const {return cfg;};

    uint32_t stepByTurn()           const {return cfg.stepByTurn;};
    uint32_t xar()                  const {return cfg.xar;};
    uint32_t yside()                const {return cfg.yside;};
    float leftWheelDiameter()    	const {return cfg.leftWheelDiameter;};
    float rightWheelDiameter()  	const {return cfg.rightWheelDiameter;};
    float voie()                 	const {return cfg.voie;};
    uint32_t maxAcc()               const {return cfg.maxAcc;};
    uint32_t maxTurnAcc()           const {return cfg.maxTurnAcc;};
    uint32_t recalSpeed()           const {return cfg.recalSpeed;};
    uint32_t maxTurnSpeed()         const {return cfg.maxTurnSpeed;};
    uint32_t maxSpeed()             const {return _maxSpeed;};
    uint32_t deccDist()             const {return cfg.deccDist;};
    uint32_t matchDuration()        const {return cfg.matchDuration;};
    uint32_t detectionWaitForOppMove()const {return cfg.detectionWaitForOppMove;};
    bool detectionActive()      	const {return cfg.detectionActive;};
    
private:
    apb_Configuration cfg;

    double _maxSpeed;               //in mm/s : maximal speed so that you match the avoidance distance

    void defaultConfig();
};

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_ROBOTCONFIG_H_ */
