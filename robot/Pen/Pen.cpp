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
        stratInstall(this),
        stratSelftest(this),
        stratWIP(this)
#endif
    {
#ifdef BUILD_STRATEGY
        //register strategies
        lifecycle.registerMatchType("Match",          &stratInstall,      &stratHomol,    NULL);
        lifecycle.registerMatchType("Homol",          &stratInstall,      &stratHomol,    NULL);
        lifecycle.registerMatchType("Selftest",       NULL,               &stratSelftest, NULL);
        lifecycle.registerMatchType("WIP",            NULL,               &stratWIP,      NULL);
        lifecycle.registerLinearStrat("Old Tanguy",   Strategy_Tanguy);
        lifecycle.registerLinearStrat("UT LEDs",      Strategy_LedTest);
        lifecycle.registerLinearStrat("UT Button",    Strategy_ButtonTest);
        lifecycle.registerLinearStrat("UT Omron",     Strategy_OmronTest);
        lifecycle.registerLinearStrat("UT CalibRot",  Strategy_CalibRot);
        lifecycle.registerLinearStrat("UT CalibLin",  Strategy_CalibLin);
        lifecycle.registerLinearStrat("UT Motion",    Strategy_MotionTest);
#endif

        //Retrieve and modify config
        apb_Configuration cfg = getConfig();
        strcpy(cfg.serialNumber,    "Pen");
        cfg.leftWheelDiameter       = 60.000;
        cfg.rightWheelDiameter      = 60.000;
        cfg.voie                    = 160.4;
        cfg.maxAcc                  = 700;
        cfg.maxTurnAcc              = 300;
        cfg.maxTurnSpeed            = 125.0;
        cfg.deccDist                = 150.0;
        cfg.strategyDuration        = 89500;
        cfg.detectionWaitForOppMove = 1000;
        setConfig(cfg);

    }

} /* namespace ard */
