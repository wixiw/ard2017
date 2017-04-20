/*
 * Invade.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "Invade.h"
#ifdef BUILD_STRATEGY
using namespace ard;

Invade::Invade(TimerInterface& timer):
        Strategy2017(),
        fsm()
{
    //fsm.setTimer(&timer);
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
