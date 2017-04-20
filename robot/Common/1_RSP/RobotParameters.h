/*
 * RobotParameters.h
 *
 *  Created on: 2 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_ROBOTPARAMETERS_H_
#define ROBOT_COMMON_1_RSP_ROBOTPARAMETERS_H_

#include <Com.h>

namespace ard
{

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

    uint32_t stepByTurn()           const;
    uint32_t xav()                  const;
    uint32_t xar()                  const;
    uint32_t yside()                const;
    float leftWheelDiameter()    	const;
    float rightWheelDiameter()  	const;
    float voie()                 	const;
    uint32_t maxAcc()               const;
    uint32_t maxTurnAcc()           const;
    uint32_t recalSpeed()           const;
    uint32_t maxTurnSpeed()         const;
    uint32_t deccDist()             const;
    uint32_t strategyDuration()     const;
    uint32_t detectionWaitForOppMove()const;
    bool detectionActive()      	const;
    uint32_t maxSpeed()             const;
    char const* const serialNumber() const;

    void set_stepByTurn(uint32_t value);
    void set_xav(uint32_t value);
    void set_xar(uint32_t value);
    void set_yside(uint32_t value);
    void set_leftWheelDiameter(float value);
    void set_rightWheelDiameter(float value);
    void set_voie(float value);
    void set_maxAcc(uint32_t value);
    void set_maxTurnAcc(uint32_t value);
    void set_recalSpeed(uint32_t value);
    void set_maxTurnSpeed(uint32_t value);
    void set_deccDist(uint32_t value);
    void set_strategyDuration(uint32_t value);
    void set_detectionWaitForOppMove(uint32_t value);
    void set_detectionActive(bool value);

    static const uint32_t MINBOUND_MAXSPEED         = 50;   //mm/s
    static const uint32_t MAXBOUND_MAXSPEED         = 2000; //mm/s
    static const uint32_t MINBOUND_MAXACC           = 300;  //mm/s²
    static const uint32_t MAXBOUND_MAXACC           = 2000; //mm/s²
    static const uint32_t MINBOUND_MAXTURNSPEED     = 45;   //°/s
    static const uint32_t MAXBOUND_MAXTURNSPEED     = 800;  //°/s
    static const uint32_t MINBOUND_MAXTURNACC       = 200;  //°/s²
    static const uint32_t MAXBOUND_MAXTURNACC       = 2000; //°/s²

private:
    apb_Configuration cfg;

    double m_maxSpeed;               //in mm/s : maximal speed so that you match the avoidance distance
    bool m_configuredOnce;

    void setComputedVars();
    bool checkConfig(apb_Configuration const& newConf);
};

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_ROBOTPARAMETERS_H_ */
