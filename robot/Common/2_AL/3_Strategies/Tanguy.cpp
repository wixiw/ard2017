/*
 * Tanguy.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "Tanguy.h"
#ifdef BUILD_STRATEGY
using namespace ard;

Tanguy::Tanguy(TimerInterface& timer):
        Strategy2017(),
        fsm()
{
    //fsm.setTimer(&timer);
}

void Tanguy::init()
{
    Strategy2017::init();
    fsm.init();
}

void Tanguy::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

#endif
