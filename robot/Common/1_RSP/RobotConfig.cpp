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
    cfg.maxAcc                  = 1750;
    cfg.recalSpeed              = 210.0;
    cfg.maxTurnSpeed            = 125.0;
    cfg.maxSpeed                = 700.0;
    cfg.matchDuration           = 90000000;
    cfg.detectionWaitForOppMove = 1000;
    cfg.detectionActive         = true;
}

void RobotConfig::updateConfig(apb_Configuration const& newConf)
{
	cfg = newConf;

	GAIN_MM_2_STEPS_LEFT    = - cfg.stepByTurn / (cfg.leftWheelDiameter * M_PI);
    GAIN_MM_2_STEPS_RIGHT   = cfg.stepByTurn / (cfg.rightWheelDiameter * M_PI);
    GAIN_STEPS_2_MM_LEFT    = - (cfg.leftWheelDiameter * M_PI) / cfg.stepByTurn;
    GAIN_STEPS_2_MM_RIGHT   = (cfg.rightWheelDiameter * M_PI) /  cfg.stepByTurn;
    GAIN_DEG_2_MM_LEFT      = DEG_TO_RAD * cfg.voie * GAIN_MM_2_STEPS_LEFT;
    GAIN_DEG_2_MM_RIGHT     = DEG_TO_RAD * cfg.voie * GAIN_MM_2_STEPS_LEFT;
    GAIN_RAD_2_MM_LEFT      = cfg.voie * GAIN_MM_2_STEPS_LEFT;
    GAIN_RAD_2_MM_RIGHT     = cfg.voie * GAIN_MM_2_STEPS_RIGHT;
}