/*
 * Tration.cpp
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#include "Tration.h"

using namespace ard;
Tration::Tration():
    Robot2017(),
    lsaList(*this),
    stratHomol      (*this, lsaList),
    stratInstall    (*this, lsaList),
    stratSelftest   (*this, lsaList),
    stratWIP        (*this, lsaList),
    stratFunnyAction(*this)
{
    exeBuildDate = String(__DATE__) + " " + __TIME__;
}

void Tration::init(Robot2017Listener* client)
{
    //Parent first
    Robot2017::init(client);

    //register strategies
    lifecycle.registerSelftest(&stratSelftest);
    lifecycle.registerMatchType("Match",          &stratInstall,      &stratHomol,    &stratFunnyAction);
    lifecycle.registerMatchType("Homol",          &stratInstall,      &stratHomol,    &stratFunnyAction);
    lifecycle.registerMatchType("Selftest",       NULL,               &stratSelftest, NULL);
    lifecycle.registerMatchType("WIP",            &stratInstall,      &stratWIP,      NULL);
    lifecycle.registerLinearStrat("Old Tanguy",   Strategy_Tanguy);
    lifecycle.registerLinearStrat("UT LEDs",      Strategy_LedTest);
    lifecycle.registerLinearStrat("UT Button",    Strategy_ButtonTest);
    lifecycle.registerLinearStrat("UT Omron",     Strategy_OmronTest);
    lifecycle.registerLinearStrat("UT CalibRot",  Strategy_CalibRot);
    lifecycle.registerLinearStrat("UT CalibLin",  Strategy_CalibLin);
    lifecycle.registerLinearStrat("UT Motion",    Strategy_MotionTest);

    //Retrieve and modify config
    apb_Configuration cfg = getConfig();
    strcpy(cfg.serialNumber,    "Tration");
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

String const& Tration::getExeVersion()
{
    return exeBuildDate;
}


LSA& Tration::getLSA(uint8_t id) const
{
    return lsaList.getLSA(id);
}
