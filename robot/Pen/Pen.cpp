/**
 * Pen.cpp
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#include "Pen.h"

using namespace ard;

Pen::Pen():
    Robot2017(),
    lsaList(*this),
    stratHomol      (*this, lsaList),
    stratInstall    (*this),
    stratSelftest   (*this),
    stratWIP        (*this, lsaList),
    stratFunnyAction(*this)
{
    exeBuildDate = String(__DATE__) + " " + __TIME__;
}

void Pen::init(Robot2017Listener* client)
{
    //Parent first
    Robot2017::init(client);

    //register strategies
    lifecycle.registerSelftest(&stratSelftest);
    lifecycle.registerMatchType("Match",          &stratInstall,      &stratHomol,    &stratFunnyAction);
    lifecycle.registerMatchType("Homol",          &stratInstall,      &stratHomol,    &stratFunnyAction);
    lifecycle.registerMatchType("Selftest",       NULL,               &stratSelftest, NULL);
    lifecycle.registerMatchType("WIP",            &stratInstall,      &stratWIP,      NULL);
//    lifecycle.registerLinearStrat("UT LEDs",      Strategy_LedTest);
//    lifecycle.registerLinearStrat("UT Button",    Strategy_ButtonTest);
//    lifecycle.registerLinearStrat("UT Omron",     Strategy_OmronTest);
//    lifecycle.registerLinearStrat("UT Motion",    Strategy_MotionTest);

    //Retrieve and modify config
    apb_Configuration cfg = getConfig();
    strcpy(cfg.serialNumber,    "Pen");
    cfg.leftWheelDiameter       = 59.340; //Calib 19/may and 23/may
    cfg.rightWheelDiameter      = 59.340; //Calib 19/may and 23/may
    cfg.voie                    = 160;    //Calib 19/may and 23/may
    cfg.bipFreq					= 3000;
    setConfig(cfg);
}

String const& Pen::getExeVersion()
{
    return exeBuildDate;
}

LSA& Pen::getLSA(uint8_t id) const
{
    return lsaList.getLSA(id);
}
