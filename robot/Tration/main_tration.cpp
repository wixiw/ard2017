/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "Robot2017.h"

using namespace ard;

/**
 * The little 2016 PMI
 */

int main( void )
{   
    Robot2017& robotTration = Robot2017::getInstance();

    //Retrieve and modify config
    apb_Configuration cfg = robotTration.getConfig();
    strcpy(cfg.serialNumber,    "Tration");
    cfg.stepByTurn              = 1600;
    cfg.xav                     = 117;//mm
    cfg.xar                     = 34;//mm
    cfg.yside                   = 62;
    cfg.leftWheelDiameter       = 60.000;
    cfg.rightWheelDiameter      = 60.000;
    cfg.voie                    = 108.58;
    cfg.maxAcc                  = 700;
    cfg.maxTurnAcc              = 300;
    cfg.recalSpeed              = 210.0;
    cfg.maxTurnSpeed            = 125.0;
    cfg.deccDist                = 150.0;
    cfg.strategyDuration        = 89500;
    cfg.detectionWaitForOppMove = 1000;
    cfg.detectionActive         = true;
    robotTration.setConfig(cfg);

    //Build and start robot
    robotTration.bootOs();

    return 0;
}

extern String getExeVersion()
{
    return String("Version Tration : ") + __DATE__ + " " + __TIME__;
}

