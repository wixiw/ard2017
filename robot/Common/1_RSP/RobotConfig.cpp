/*
 * RobotConfig.cpp
 *
 *  Created on: 2 avr. 2017
 *      Author: wix
 */

#include "RobotConfig.h"
#include "K_constants.h"

using namespace ard;

RobotConfig::RobotConfig()
{
    defaultConfig();
    updateConfig(cfg);
}

void RobotConfig::defaultConfig()
{
    cfg.stepByTurn              = 1600;
    cfg.xar                     = 34;
    cfg.yside                   = 60;
    cfg.leftWheelDiameter       = 60.000;
    cfg.rightWheelDiameter      = 60.000;
    cfg.voie                    = 104.73;
    cfg.maxAccFront             = 700;
    //cfg.maxDeccFront            = 700;
    cfg.recalSpeed              = 210.0;
    cfg.maxTurnSpeed            = 125.0;
    cfg.deccDist                = 150.0;
    cfg.matchDuration           = 90000000;
    cfg.detectionWaitForOppMove = 1000;
    cfg.detectionActive         = true;
}

void RobotConfig::updateConfig(apb_Configuration const& newConf)
{
	cfg = newConf;

	GAIN_MM_2_STEPS_LEFT    = -( cfg.stepByTurn / (cfg.leftWheelDiameter * M_PI));
    GAIN_MM_2_STEPS_RIGHT   = cfg.stepByTurn / (cfg.rightWheelDiameter * M_PI);
    GAIN_STEPS_2_MM_LEFT    = 1/GAIN_MM_2_STEPS_LEFT;
    GAIN_STEPS_2_MM_RIGHT   = 1/GAIN_MM_2_STEPS_RIGHT;
    GAIN_RAD_2_STEPS_LEFT   = cfg.voie * GAIN_MM_2_STEPS_LEFT / 2.;
    GAIN_RAD_2_STEPS_RIGHT  = cfg.voie * GAIN_MM_2_STEPS_RIGHT / 2.;

    maxSpeedFront = sqrt(2*cfg.maxAccFront*cfg.deccDist);
    maxSpeedRear = sqrt(2*cfg.maxAccFront*cfg.deccDist);
}
