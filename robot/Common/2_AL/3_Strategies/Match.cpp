/*
 * Match.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "Match.h"
#ifdef BUILD_STRATEGY
using namespace ard;

Match::Match(TimerInterface& timer):
        Strategy2017(),
        fsm()
{
    //fsm.setTimer(&timer);
}

void Match::init()
{
    Strategy2017::init();
    fsm.init();
}

void Match::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

#endif
