/*
 * WipTration.cpp
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#include "WipTration.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

WipTration::WipTration(Robot2017* robot):
        Strategy2017(robot),
        fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
}

void WipTration::init()
{
    Strategy2017::init();
    fsm.init();
}

void WipTration::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

#endif
