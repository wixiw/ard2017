/*
 * Installation.cpp
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#include "Installation.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

InstallPen::InstallPen(Robot2017* robot):
        Strategy2017(robot, "InstallPen"),
        fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
    //fsm.setDefaultSCI_OCB(this);
}

void InstallPen::init()
{
    Strategy2017::init();
    fsm.init();
    fsm.enter();
}

void InstallPen::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

STRAT_2017_API_IMPL(InstallPen);

InstallTration::InstallTration(Robot2017* robot):
        Strategy2017(robot, "InstallTration"),
        fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
    //fsm.setDefaultSCI_OCB(this);
}

void InstallTration::init()
{
    Strategy2017::init();
    fsm.init();
}

void InstallTration::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

STRAT_2017_API_IMPL(InstallTration);

#endif
