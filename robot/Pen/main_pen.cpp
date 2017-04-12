/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "Robot2017.h"

using namespace ard;

/**
 * The new 2017 robot
 */

int main( void )
{   
    Robot2017& robotPen = Robot2017::getInstance();

    //Retrieve and modify config
    apb_Configuration cfg = robotPen.getConfig();
    strcpy(cfg.serialNumber,    "Pen");
    cfg.stepByTurn              = 1600;
    cfg.xav                     = 145;//mm
    cfg.xar                     = 30;//mm
    cfg.yside                   = 88;
    cfg.leftWheelDiameter       = 60.000;
    cfg.rightWheelDiameter      = 60.000;
    cfg.voie                    = 160.4;
    cfg.maxAcc                  = 700;
    cfg.maxTurnAcc              = 300;
    cfg.recalSpeed              = 210.0;
    cfg.maxTurnSpeed            = 125.0;
    cfg.deccDist                = 150.0;
    cfg.matchDuration           = 90000;
    cfg.detectionWaitForOppMove = 1000;
    cfg.detectionActive         = true;
    robotPen.setConfig(cfg);

    //Build and start robot
    robotPen.bootOs();

    return 0;
}

extern String getExeVersion()
{
    return String("Version Pen : ") + __DATE__ + " " + __TIME__;
}

