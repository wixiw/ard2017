/*
 * Pen.cpp
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#include "Pen.h"

namespace ard
{
    //singleton instanciation
    Pen* Pen::instance2 = NULL;

    Pen::Pen():
        Robot2017()
#ifdef BUILD_STRATEGY
        , stratHomol(this),
        stratInstallation(this),
        stratMatch(this),
        stratSelftest(this),
        stratWIP(this)
#endif
    {
#ifdef BUILD_STRATEGY
        //register strategies
        lifecycle.registerMatchType("Match",          &stratInstallation, &stratMatch,    NULL);
        lifecycle.registerMatchType("Homol",          &stratInstallation, &stratHomol,    NULL);
        lifecycle.registerMatchType("Selftest",       &stratSelftest,     NULL,           NULL);
        lifecycle.registerMatchType("WIP",            NULL,               &stratWIP,      NULL);
        lifecycle.registerLinearStrat("Old Tanguy",   Strategy_Tanguy);
//    lifecycle.registerStrategy("UT LEDs",        Strategy_LedTest);
//    lifecycle.registerStrategy("UT Button",      Strategy_ButtonTest);
//    lifecycle.registerStrategy("UT Omron",       Strategy_OmronTest);
//    lifecycle.registerStrategy("UT CalibRot",    Strategy_CalibRot);
//    lifecycle.registerStrategy("UT CalibLin",    Strategy_CalibLin);
//    lifecycle.registerStrategy("UT Motion",      Strategy_MotionTest);
#endif

        //Retrieve and modify config
        apb_Configuration cfg = getConfig();
        strcpy(cfg.serialNumber,    "Pen");
        cfg.stepByTurn              = 1600;
        cfg.xav                     = 145;//mm
        cfg.xar                     = 30;//mm
        cfg.yside                   = 88;
        cfg.xavExtended             = 210;//mm
        cfg.leftWheelDiameter       = 60.000;
        cfg.rightWheelDiameter      = 60.000;
        cfg.voie                    = 160.4;
        cfg.maxAcc                  = 700;
        cfg.maxTurnAcc              = 300;
        cfg.recalSpeed              = 210.0;
        cfg.maxTurnSpeed            = 125.0;
        cfg.deccDist                = 150.0;
        cfg.strategyDuration        = 89500;
        cfg.detectionWaitForOppMove = 1000;
        cfg.detectionActive         = true;
        setConfig(cfg);

    }

} /* namespace ard */
