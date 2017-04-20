/*
 * Installation.cpp
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#include "Installation.h"
#ifdef BUILD_STRATEGY
using namespace ard;

Installation::Installation(TimerInterface& timer):
        Strategy2017(),
        fsm()
{
    //fsm.setTimer(&timer);
}

void Installation::init()
{
    Strategy2017::init();
    fsm.init();
}

void Installation::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

#endif
