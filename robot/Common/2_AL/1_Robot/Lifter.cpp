/*
 * Lifter.cpp
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#include "Lifter.h"
#include "ActuatorThread.h"

using namespace ard;


Lifter::Lifter(ActuatorThread& parent, TimerInterface& timer, HmiThread& hmi):
        fsm(),
        acts(parent),
		hmi(hmi)
{
    fsm.setTimer(&timer);
    fsm.setDefaultSCI_OCB(this);
}

void Lifter::init()
{
    fsm.init();
    fsm.enter();

    //Put an initial value for outputs which are not stupid
	fsm.set_servoCmd(500);

	ASSERT_TEXT(UP_CMD() < DOWN_CMD() && DOWN_CMD() < FULL_DOWN_CMD(), "Servo commands value are not correct");
}

void Lifter::start()
{
	LOG_INFO("[Lifter] start request");
    fsm.getSCI_Strategy()->raise_start();
}

void Lifter::lift()
{
	LOG_INFO("[Lifter] lift request");
    fsm.getSCI_Strategy()->raise_lift();
}

void Lifter::prepareNextToPoo()
{
	LOG_INFO("[Lifter] prepareNextToPoo request");
    fsm.getSCI_Strategy()->raise_prepareNextToPoo();
}

void Lifter::stop()
{
	LOG_INFO("[Lifter] stop request");
    fsm.getSCI_Strategy()->raise_stop();
    acts.servoLifter.disable();
    fsm.set_started(false);
}

void Lifter::publishTimeoutOnHMI()
{
	hmi.buzzer.bip(4);
}

void Lifter::publishBlockedOnHMI()
{
	hmi.buzzer.playTone(3200, 3000);
}

void Lifter::logInfo(sc_string msg)
{
	LOG_INFO(String("[Lifter] ") + msg);
}

void Lifter::logError(sc_string msg)
{
	LOG_ERROR(String("[Lifter] ") + msg);
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

    if(fsm.getSCI_Strategy()->get_blocked())
    	acts.servoLifter.disable();
}

//configure if component is simulated or not
void Lifter::setSimulation(bool simulated)
{
	fsm.set_simulation(simulated);
}

