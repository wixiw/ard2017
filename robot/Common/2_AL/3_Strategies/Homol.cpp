/*
 * Homol.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "Homol.h"
#ifdef BUILD_STRATEGY
using namespace ard;

Homol::Homol(TimerInterface& timer):
        Strategy2017(),
        fsm()
{
    //fsm.setTimer(&timer);
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
