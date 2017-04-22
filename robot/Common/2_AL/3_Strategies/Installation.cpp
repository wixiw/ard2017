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
        Strategy2017(robot),
        fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
}

void InstallPen::init()
{
    Strategy2017::init();
    fsm.init();
}

void InstallPen::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

InstallTration::InstallTration(Robot2017* robot):
Strategy2017(robot),
fsm()
{
    //fsm.setTimer(&robot->fsmTimer);
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

#endif
