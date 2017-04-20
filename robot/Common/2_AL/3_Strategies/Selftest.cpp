/*
 * Selftest.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "Selftest.h"
#ifdef BUILD_STRATEGY
using namespace ard;

Selftest::Selftest(TimerInterface& timer):
        Strategy2017(),
        fsm()
{
    //fsm.setTimer(&timer);
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
