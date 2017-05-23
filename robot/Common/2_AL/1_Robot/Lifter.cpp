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
    fsm.setDefaultSCI_OCB(this);
}

void Lifter::init()
{
    fsm.init();
    fsm.enter();

    //TODO better : replace constants in FSM by variables set here
    ASSERT_TEXT(LIFTER_MIN == fsm.get_uP_CMD(), "RobotParameters and FSM_Lifter.sct have diverged");
    ASSERT_TEXT(LIFTER_MAX == fsm.get_fULL_DOWN_CMD(), "RobotParameters and FSM_Lifter.sct have diverged");
	
	fsm.set_servoCmd(500);
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

void Lifter::fastPoo()
{
	LOG_INFO("[Lifter] fast poo request");
    fsm.getSCI_Strategy()->raise_fastPoo();
}

void Lifter::pooEnded()
{
	LOG_INFO("[Lifter] poo ended request");
    fsm.getSCI_Strategy()->raise_pooEnded();
}

void Lifter::stop()
{
	LOG_INFO("[Lifter] stop request");
    fsm.getSCI_Strategy()->raise_stop();
    acts.servoLifter.disable();
    fsm.set_started(false);
}

void Lifter::blocked()
{
	LOG_INFO("Lifter blocked.");
	acts.servoLifter.disable();
	fsm.set_started(false);
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
}

//configure if component is simulated or not
void Lifter::setSimulation(bool simulated)
{
	fsm.set_simulation(simulated);
}

