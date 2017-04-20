/*
 * Willy.cpp
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#include "Willy.h"
#ifdef BUILD_STRATEGY
using namespace ard;

Willy::Willy(TimerInterface& timer):
        Strategy2017(),
        fsm()
{
    fsm.setTimer(&timer);
}

void Willy::init()
{
    Strategy2017::init();
    fsm.init();
}

void Willy::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

#endif
