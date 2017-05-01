/*
 * Lifter.cpp
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#include "../1_Robot/Lifter.h"

#include "../1_Robot/ActuatorThread.h"

using namespace ard;


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
        acts.servoLifter.goTo(fsm.get_lifter());
    }
}


