/*
 * Lifter.cpp
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#include "Lifter.h"
#include "ActuatorThread.h"

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
}

void Lifter::update(TimeMs sinceLastCall)
{
    fsm.getDefaultSCI()->set_downSwitch(acts.switchLifterDown.read());
    fsm.getDefaultSCI()->set_upSwitch(acts.switchLifterUp.read());

    fsm.runCycle();

    //TODO convert 0=>100% to 0=>180
    acts.servoLifter.write(fsm.getDefaultSCI()->get_lifter());
}

