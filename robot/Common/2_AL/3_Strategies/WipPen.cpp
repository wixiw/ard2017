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
        Strategy2017(robot),
        fsm()
{
    //fsm.setTimer&(robot->fsmTimer);
}

void WipPen::init()
{
    Strategy2017::init();
    fsm.init();
}

void WipPen::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

#endif
