/*
 * Lifter.cpp
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#include "Lifter.h"
#include "ActuatorThread.h"

using namespace ard;

#ifdef BUILD_STRATEGY

Lifter::Lifter(ActuatorThread& parent, TimerInterface& timer):
        fsm(),
        acts(parent)
{
    fsm.setTimer(&timer);
}

void Lifter::init()
{
    fsm.init();
    fsm.enter();
}

void Lifter::start()
{
    NOT_IMPLEMENTED(); //TODO : corriger les valeurs des servos avant d'utiliser
    fsm.set_start(true);
}

void Lifter::update(TimeMs sinceLastCall)
{
    fsm.set_downSwitch(acts.switchLifterDown.read());
    fsm.set_upSwitch(acts.switchLifterUp.read());

    fsm.runCycle();

    if( fsm.get_start() )
    {
        acts.servoLifter.write(fsm.get_lifter());
    }
}

#endif

