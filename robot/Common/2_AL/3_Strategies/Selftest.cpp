/*
 * Selftest.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "Selftest.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

Selftest::Selftest(Robot2017* robot):
        Strategy2017(robot),
        fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
}

void Selftest::init()
{
    Strategy2017::init();
    fsm.init();
}

void Selftest::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

#endif
