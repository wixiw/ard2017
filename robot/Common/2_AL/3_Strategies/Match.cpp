/*
 * Match.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "Match.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

Match::Match(Robot2017* robot):
        Strategy2017(robot),
        fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
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
