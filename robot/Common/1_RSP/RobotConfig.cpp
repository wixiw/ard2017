/*
 * RobotConfig.cpp
 *
 *  Created on: 2 avr. 2017
 *      Author: wix
 */

#include "RobotConfig.h"
#include "K_constants.h"
#include "core/ArdMaths.h"
#include "Log.h"

using namespace ard;

RobotConfig::RobotConfig()
{
    defaultConfig();
    ASSERT(checkConfig(cfg));
}

void RobotConfig::defaultConfig()
{
    cfg.stepByTurn              = 1600;
    cfg.xar                     = 34;
    cfg.yside                   = 60;
    cfg.leftWheelDiameter       = 60.000;
    cfg.rightWheelDiameter      = 60.000;
    cfg.voie                    = 104.73;
    cfg.maxAcc                  = 700;
    cfg.maxTurnAcc              = 300;
    cfg.recalSpeed              = 210.0;
    cfg.maxTurnSpeed            = 125.0;
    cfg.deccDist                = 150.0;
    cfg.matchDuration           = 90000;
    cfg.detectionWaitForOppMove = 1000;
    cfg.detectionActive         = true;
    setComputedVars();
}

void RobotConfig::setComputedVars()
{
    GAIN_MM_2_STEPS_LEFT    = -( cfg.stepByTurn / (cfg.leftWheelDiameter * M_PI));
    GAIN_MM_2_STEPS_RIGHT   = cfg.stepByTurn / (cfg.rightWheelDiameter * M_PI);
    GAIN_STEPS_2_MM_LEFT    = 1/GAIN_MM_2_STEPS_LEFT;
    GAIN_STEPS_2_MM_RIGHT   = 1/GAIN_MM_2_STEPS_RIGHT;
    GAIN_RAD_2_STEPS_LEFT   = cfg.voie * GAIN_MM_2_STEPS_LEFT / 2.;
    GAIN_RAD_2_STEPS_RIGHT  = cfg.voie * GAIN_MM_2_STEPS_RIGHT / 2.;
    _maxSpeed               = sqrt(2*cfg.maxAcc*cfg.deccDist);
}

#define CHECK_RANGE(min, value, max) IS_OUT_RANGE(min, value, max) \
{ \
    LOG_ERROR(String(#value) + " is out of acceptable range [" + min + ", " +  max + "].");    \
    res &= false;   \
}

bool RobotConfig::checkConfig(apb_Configuration const& newConf)
{
    bool res = true;
    double newMaxSpeed = sqrt(2*cfg.maxAcc*cfg.deccDist);

    //Checks :
    CHECK_RANGE(5, newConf.xar, 500);
    CHECK_RANGE(5, newConf.yside, 500);
    CHECK_RANGE(50, newConf.leftWheelDiameter, 70);
    CHECK_RANGE(50, newConf.rightWheelDiameter, 70);
    CHECK_RANGE(50, newConf.voie, 300);
    CHECK_RANGE(MINBOUND_MAXACC, newConf.maxAcc, MAXBOUND_MAXACC);
    CHECK_RANGE(MINBOUND_MAXTURNSPEED, newConf.maxTurnSpeed, MAXBOUND_MAXTURNSPEED);
    CHECK_RANGE(50, newConf.recalSpeed, 500);
    CHECK_RANGE(MINBOUND_MAXTURNACC, newConf.maxTurnAcc, MAXBOUND_MAXTURNACC);
    CHECK_RANGE(20, newConf.deccDist, 250);
    if( newConf.matchDuration )
    {
        CHECK_RANGE(50000, newConf.matchDuration, 0xFFFFFFFF);
    }
    CHECK_RANGE(50, newConf.detectionWaitForOppMove, 5000);
    CHECK_RANGE(MINBOUND_MAXSPEED, newMaxSpeed, MAXBOUND_MAXSPEED);

    return res;
}

bool RobotConfig::updateConfig(apb_Configuration const& newConf)
{
    //update config.
    if( checkConfig(newConf) )
    {
        cfg = newConf;
        setComputedVars();
        LOG_INFO("New configuration applied");
        return true;
    }
    else
    {
        LOG_ERROR("Configuration not applied");
        return false;
    }
}
