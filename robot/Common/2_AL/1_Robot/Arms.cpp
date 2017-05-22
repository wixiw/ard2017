/*
 * Arms.cpp
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#include "Arms.h"
#include "ActuatorThread.h"

using namespace ard;

Arms::Arms(ActuatorThread& parent, TimerInterface& timer):
    fsm(),
    acts(parent),
    lifter(NULL)
{
    fsm.setTimer(&timer);
    fsm.setDefaultSCI_OCB(this);
}

void Arms::init()
{
    ASSERT(lifter!=NULL);
    fsm.init();
    fsm.enter();
}

void Arms::start()
{
    fsm.getSCI_Strategy()->raise_start();
}

void Arms::swallow()
{
    fsm.getSCI_Strategy()->raise_swallow();
}

void Arms::retract()
{
    fsm.getSCI_Strategy()->raise_retractArms();
}

void Arms::fastPoo(uint8_t nbCylinders)
{
    fsm.getSCI_Strategy()->raise_fastPoo(nbCylinders);
}

void Arms::stop()
{
    fsm.getSCI_Strategy()->raise_stop();
    acts.servoLeftArm.disable();
    acts.servoRightArm.disable();
    acts.servoLeftWheel.disable();
    acts.servoRightWheel.disable();
    fsm.set_started(false);
}

void Arms::blocked()
{
	LOG_INFO("Arms blocked.");
    acts.servoLeftArm.disable();
    acts.servoRightArm.disable();
    acts.servoLeftWheel.disable();
    acts.servoRightWheel.disable();
	fsm.set_started(false);
}

void Arms::lift()
{
	acts.actCmd(eActCmd_AC_LIFTER_LIFT);
}

void Arms::logInfo(sc_string msg)
{
	LOG_INFO(String("[Arms] ") + msg);
}

void Arms::logError(sc_string msg)
{
	LOG_ERROR(String("[Arms] ") + msg);
}

void Arms::update(TimeMs sinceLastCall)
{
    fsm.set_leftExtendedSwitch(acts.switchArmLout.read());
    fsm.set_rightExtendedSwitch(acts.switchArmRout.read());
    fsm.set_leftRetractedSwitch(acts.switchArmLin.read());
    fsm.set_rightRetractedSwitch(acts.switchArmRin.read());
    fsm.set_cylinderPresent(acts.omronCylinder.read());
    fsm.set_lifterReady(lifter->isReady());
    fsm.set_armsInPosition(acts.servoLeftArm.isTargetReached()&&acts.servoRightArm.isTargetReached());

    fsm.runCycle();

    if( fsm.get_started() )
    {
        acts.servoLeftArm.      goTo(fsm.get_leftArm());
        acts.servoRightArm.     goTo(fsm.get_rightArm());
        acts.servoLeftWheel.    goTo(fsm.get_leftWheel());
        acts.servoRightWheel.   goTo(fsm.get_rightWheel());
    }
}

//configure if component is simulated or not
void Arms::setSimulation(bool simulated)
{
	fsm.set_simulation(simulated);
}
