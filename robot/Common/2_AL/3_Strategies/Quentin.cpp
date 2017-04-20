/*
 * Quentin.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "Quentin.h"
#ifdef BUILD_STRATEGY
using namespace ard;

Quentin::Quentin(TimerInterface& timer):
        Strategy2017(),
        fsm()
{
    //fsm.setTimer(&timer);
}

void Quentin::init()
{
    Strategy2017::init();
    fsm.init();
}

void Quentin::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

#endif
