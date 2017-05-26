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
    stratInstall    (*this),
    stratSelftest   (*this),
//    stratWIP        (*this, lsaList),
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
    lifecycle.registerMatchType("Selftest",       &stratInstall,      &stratHomol,    &stratFunnyAction);
    lifecycle.registerMatchType("WIP",            &stratInstall,      &stratHomol,    &stratFunnyAction);
//    lifecycle.registerLinearStrat("UT LEDs",      Strategy_LedTest);
//    lifecycle.registerLinearStrat("UT Button",    Strategy_ButtonTest);
//    lifecycle.registerLinearStrat("UT Omron",     Strategy_OmronTest);
//    lifecycle.registerLinearStrat("UT Motion",    Strategy_MotionTest);

    //Retrieve and modify config
    apb_Configuration cfg = getConfig();
    strcpy(cfg.serialNumber,    "Tration");
    cfg.leftWheelDiameter       = 59.4313; //Calib 26/may 6h après mise rouleau
    cfg.rightWheelDiameter      = 59.4313; //Calib 26/may 6h après mise rouleau
    cfg.voie                    = 160.26;  //Calib 26/may 6h après mise rouleau
    cfg.bipFreq					= 3000;
    cfg.bipAllowed				= true; //pour la nuit
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
