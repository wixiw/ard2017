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
        Strategy2017(robot, "WipTration"),
        fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
    //fsm.setDefaultSCI_OCB(this);
}

void WipTration::init()
{
    Strategy2017::init();
    fsm.init();
    fsm.enter();
}

void WipTration::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

STRAT_2017_API_IMPL(WipTration);

#endif
