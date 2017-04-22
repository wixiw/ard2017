/*
 * Invade.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "Invade.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

Invade::Invade(Robot2017* robot):
        Strategy2017(robot),
        fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
}

void Invade::init()
{
    Strategy2017::init();
    fsm.init();
}

void Invade::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

#endif
