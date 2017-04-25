/*
 * HomolPen.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "HomolPen.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

HomolPen::HomolPen(Robot2017* robot):
        Strategy2017(robot, "HomolPen"),
        fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
    //fsm.setDefaultSCI_OCB(this);
}

void HomolPen::init()
{
    Strategy2017::init();
    fsm.init();
    fsm.enter();
}

void HomolPen::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

STRAT_2017_API_IMPL(HomolPen);


#endif
