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
    fsm.enter();
}

void Lifter::start()
{
    fsm.getSCI_Strategy()->raise_start();
}

void Lifter::lift()
{
    fsm.getSCI_Strategy()->raise_lift();
}

void Lifter::fastPoo()
{
    fsm.getSCI_Strategy()->raise_fastPoo();
}

void Lifter::pooEnded()
{
    fsm.getSCI_Strategy()->raise_pooEnded();
}

void Lifter::stop()
{
    fsm.getSCI_Strategy()->raise_stop();
}

void Lifter::update(TimeMs sinceLastCall)
{
    fsm.set_downSwitch(acts.switchLifterDown.read());
    fsm.set_upSwitch(acts.switchLifterUp.read());
    fsm.set_cylinderPresent(acts.omronCylinder.read());
    fsm.set_targetReached(acts.servoLifter.isTargetReached());

    fsm.runCycle();

    if(fsm.get_started())
    	acts.servoLifter.goTo(fsm.get_servoCmd());
}


