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

    ASSERT_TEXT(ARM_FULL_IN() < ARM_MID() && ARM_MID() < ARM_OUT(), "Servo commands value are not correct");

    //Put an initial value for outputs which are not stupid
    armsCmd(ARM_MID(), WHEEL_IDLE());
    rotatorCmd(ROT_RETRACTED(), false);
}

void Arms::start()
{
	LOG_INFO("[Arms] start request");
    fsm.getSCI_Strategy()->raise_start();
}

void Arms::swallowNormal()
{
	LOG_INFO("[Arms] swallowNormal request");
    fsm.getSCI_Strategy()->raise_swallowNormal();
}

void Arms::swallowDispenser()
{
	LOG_INFO("[Arms] swallowDispenser request");
    fsm.getSCI_Strategy()->raise_swallowDispenser();
}

void Arms::retract()
{
	LOG_INFO("[Arms] retract request");
    fsm.getSCI_Strategy()->raise_retractArms();
}

void Arms::fastPoo()
{
	LOG_INFO("[Arms] fastPoo request");
    fsm.getSCI_Strategy()->raise_fastPoo();
}

void Arms::stop()
{
	LOG_INFO("[Arms] stop request");
    fsm.getSCI_Strategy()->raise_stop();
    disableAll();
    fsm.set_started(false);
}

void Arms::lift()
{
	acts.actCmd(eActCmd_AC_LIFTER_LIFT);
}

void Arms::prepareNextToPoo()
{
	acts.actCmd(eActCmd_AC_LIFTER_PREPARE_NEXT_TO_POO);
}

void Arms::lifterRearm()
{
	acts.actCmd(eActCmd_AC_LIFTER_START);
}

void Arms::logInfo(sc_string msg)
{
	LOG_INFO(String("[Arms] ") + msg);
}

void Arms::logError(sc_string msg)
{
	LOG_ERROR(String("[Arms] ") + msg);
}

void Arms::armsCmd(sc_integer arms, sc_integer wheels)
{
    acts.servoLeftArm.      goTo(arms);
    acts.servoRightArm.     goTo(arms);
    acts.servoLeftWheel.    goTo(wheels);
    acts.servoRightWheel.   goTo(wheels);
}

void Arms::rotatorCmd(sc_integer servo, bool turn)
{
	acts.servoRotator.goTo(servo);
	acts.rotatorMotor.turn(turn);
}

void Arms::update(TimeMs sinceLastCall)
{
	//read inputs
	fsm.set_cylinderPresent(acts.omronCylinder.read());
    fsm.set_leftExtendedSwitch(acts.switchArmLout.read());
    fsm.set_rightExtendedSwitch(acts.switchArmRout.read());
    fsm.set_leftRetractedSwitch(acts.switchArmLin.read());
    fsm.set_rightRetractedSwitch(acts.switchArmRin.read());
    fsm.set_armsInPosition(acts.servoLeftArm.isTargetReached()&&acts.servoRightArm.isTargetReached());
    fsm.set_rotatorInPosition(acts.servoRotator.isTargetReached());

    //run
    fsm.runCycle();

    //publish/manage outputs
    if(fsm.getSCI_Strategy()->get_blocked())
    	disableAll();
}

//configure if component is simulated or not
void Arms::setSimulation(bool simulated)
{
	fsm.set_simulation(simulated);
}

void Arms::disableAll()
{
    acts.servoLeftArm.disable();
    acts.servoRightArm.disable();
    acts.servoLeftWheel.disable();
    acts.servoRightWheel.disable();
    acts.servoRotator.disable();
    acts.rotatorMotor.turn(false);
}
