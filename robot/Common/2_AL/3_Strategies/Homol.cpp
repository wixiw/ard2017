/*
 * Homol.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "Homol.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

Homol::Homol(Robot2017* robot):
        Strategy2017(robot),
        fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
}

void Homol::init()
{
    Strategy2017::init();
    fsm.init();
}

void Homol::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

#endif
