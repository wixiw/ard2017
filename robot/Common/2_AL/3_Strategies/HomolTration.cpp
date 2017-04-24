/*
 * HomolTration.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "HomolTration.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

HomolTration::HomolTration(Robot2017* robot):
        Strategy2017(robot, "HomolTration"),
        fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
    //fsm.setDefaultSCI_OCB(this);
}

void HomolTration::init()
{
    Strategy2017::init();
    fsm.init();
    fsm.enter();
}

void HomolTration::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

STRAT_2017_API_IMPL(HomolTration);

#endif
