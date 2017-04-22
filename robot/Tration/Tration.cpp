/*
 * Tration.cpp
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#include "Tration.h"

namespace ard
{
    //singleton instanciation
    Tration* Tration::instance2 = NULL;

    Tration::Tration():
        Robot2017()
#ifdef BUILD_STRATEGY
        , stratFunnyAction(this),
        stratInstallation(this),
        stratInvade(this),
        stratWIP(this)
#endif
    {
#ifdef BUILD_STRATEGY
        //register strategies
        lifecycle.registerMatchType("Invade",         &stratInstallation, &stratInvade,   &stratFunnyAction);
        lifecycle.registerMatchType("Willy",          NULL,               &stratWIP,    NULL);
    //    lifecycle.registerStrategy("UT LEDs",        Strategy_LedTest);
    //    lifecycle.registerStrategy("UT Button",      Strategy_ButtonTest);
    //    lifecycle.registerStrategy("UT Omron",       Strategy_OmronTest);
    //    lifecycle.registerStrategy("UT CalibRot",    Strategy_CalibRot);
    //    lifecycle.registerStrategy("UT CalibLin",    Strategy_CalibLin);
    //    lifecycle.registerStrategy("UT Motion",      Strategy_MotionTest);
#endif

        //Retrieve and modify config
        apb_Configuration cfg = getConfig();
        strcpy(cfg.serialNumber,    "Tration");
        cfg.stepByTurn              = 1600;
        cfg.xav                     = 117;//mm
        cfg.xar                     = 34;//mm
        cfg.yside                   = 62;
        cfg.xavExtended             = cfg.xav;
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
        setConfig(cfg);

    }

} /* namespace ard */
