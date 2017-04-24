/*
 * WipPen.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "WipPen.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

WipPen::WipPen(Robot2017* robot):
        Strategy2017(robot, "WipPen"),
        fsm()
{
    //fsm.setTimer&(robot->fsmTimer);
    fsm.setDefaultSCI_OCB(this);
}

void WipPen::init()
{
    Strategy2017::init();
    fsm.init();
    fsm.enter();
}

void WipPen::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

STRAT_2017_API_IMPL(WipPen);

#endif
