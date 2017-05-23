/*
 * ActuatorThread.cpp
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#include "ActuatorThread.h"
#include "Motion/KinematicManager.h"

using namespace ard;

ActuatorThread::ActuatorThread(KinematicManager& kinMan):
        PollerThread("Actuator", PRIO_ACTUATORS, STACK_ACTUATORS, PERIOD_ACTUATORS, 11),
        stockColor(),
        switchArmLout(      SWITCH_ARM_L_OUT, 100, 10),
        switchArmLin(       SWITCH_ARM_L_IN, 100, 10),
        switchArmRout(      SWITCH_ARM_R_OUT, 100, 10),
        switchArmRin(       SWITCH_ARM_R_IN, 100, 10),
        omronCylinder(      OMRON2, 200, 50, true), //TODO remettre OMRON_CYLINDER
        switchLifterUp(     SWITCH_LIFTER_UP, 100, 10),
        switchLifterDown(   SWITCH_LIFTER_DOWN, 100, 10),
        servoLifter("Lifter", 			SERVO1, LIFTER_MIN, LIFTER_MAX),
        servoLeftArm("LeftArm", 		SERVO2, ARM_MIN, ARM_MAX),
        servoRightArm("RightArm", 		SERVO3, ARM_MIN, ARM_MAX, true),
        servoLeftWheel("LeftWheel",     SERVO4, 0, 1000),
        servoRightWheel("RightWheel",   SERVO5, 0, 1000),
        servoFunnyAction("Funny", 		SERVO6, 0, 1000),
		servoRotator("Rotateur", 		SERVO7, 0, 1000),
		servo8("Servo8", 				SERVO8, 0, 1000),
        lifter(*this, fsmTimeWheel),
        arms(*this, fsmTimeWheel),
        faceUp(*this),
		state(),
		fsmTimeWheel(),
		kinematics(kinMan)
{
    state = apb_ActuatorsState_init_default;

    servoLifter		.setAccMax(15);
    servoLeftArm	.setAccMax(15);
    servoRightArm	.setAccMax(15);
    servoLeftWheel	.setAccMax(15);
    servoRightWheel	.setAccMax(15);
    servoFunnyAction.setAccMax(15);
    servoRotator	.setAccMax(15);
    servo8			.setAccMax(15);

    servoLifter		.setVmax(60);
    servoLeftArm	.setVmax(100);
    servoRightArm	.setVmax(100);
    servoLeftWheel	.setVmax(100);
    servoRightWheel	.setVmax(100);
    servoFunnyAction.setVmax(60);
    servoRotator	.setVmax(60);
    servo8			.setVmax(60);

}

void ActuatorThread::setColor(eColor color)
{
    faceUp.setColor(color);

}

void ActuatorThread::init()
{
	ASSERT_CONFIGURED();
    arms.setLifter(lifter);
    addPolledObject(stockColor);
    addPolledObject(lifter);
    addPolledObject(arms);
    addPolledObject(servoLifter);
    addPolledObject(servoLeftArm);
    addPolledObject(servoRightArm);
    addPolledObject(servoLeftWheel);
    addPolledObject(servoRightWheel);
    addPolledObject(servoFunnyAction);
    addPolledObject(servoRotator);
    addPolledObject(servo8);
    PollerThread::init();
    ASSERT_TEXT(conf, "You forgot to map the configuration.");
}

void ActuatorThread::run()
{
    fsmTimeWheel.run(getPeriod());
    PollerThread::run();

	uint16_t left = servoLeftArm.read();
	uint16_t right = servoRightArm.read();
    if( 400 < left || 400 < right)
    {
    	kinematics.frontActuatorsOut(true);
    }
    else
    {
    	kinematics.frontActuatorsOut(false);
    }
}

void ard::ActuatorThread::turnCylinder(bool turn)
{
    digitalWrite(MOSFET1, turn);
}

void ard::ActuatorThread::faceUpCylinder()
{
    faceUp.faceUpCylinder();
}

eFaceUpStatus ard::ActuatorThread::getFaceUpStatus()
{
    return faceUp.getFaceUpStatus();
}

apb_ActuatorsState const& ActuatorThread::serealize()
{
    state = apb_ActuatorsState_init_default;

    state.colorSensor       = stockColor.getState();
    state.switchArmLout     = switchArmLout.readRaw();
    state.switchArmLin      = switchArmLin.readRaw();
    state.switchArmRout     = switchArmRout.readRaw();
    state.switchArmRin      = switchArmRin.readRaw();
    state.omronCylinder     = omronCylinder.readRaw();
    state.switchLifterUp    = switchLifterUp.readRaw();
    state.switchLifterDown  = switchLifterDown.readRaw();

    state.servoLifter       = servoLifter.read();
    state.servoLeftArm      = servoLeftArm.read();
    state.servoRightArm     = servoRightArm.read();
    state.servoLeftWheel    = servoLeftWheel.read();
    state.servoRightWheel   = servoRightWheel.read();
    state.servoFunnyAction  = servoFunnyAction.read();
    state.servoRotator  	= servoRotator.read();
    state.servo8  			= servo8.read();

    state.lifterReady		= lifter.isReady();
    state.armsReady			= arms.isReady();


    return state;
}

void ActuatorThread::startAll()
{
	lifter.start();
	arms.start();
}

void ActuatorThread::disableAll()
{
    servoLifter.disable();
    servoLeftArm.disable();
    servoRightArm.disable();
    servoLeftWheel.disable();
    servoRightWheel.disable();
    servoRotator.disable();
    servo8.disable();
}

void ActuatorThread::actCmd(eActCmd cmd)
{
	LOG_INFO("Actuator command received : " + String((int)cmd));

	switch (cmd) {
	/**
	 * LIFTER
	 */
		case eActCmd_AC_LIFTER_START:
			lifter.start();
			break;
		case eActCmd_AC_LIFTER_STOP:
			lifter.stop();
			break;
		case eActCmd_AC_LIFTER_LIFT:
			lifter.lift();
			break;
		case eActCmd_AC_LIFTER_FASTPOO:
			lifter.fastPoo();
			break;
		case eActCmd_AC_LIFTER_POO:
			NOT_IMPLEMENTED(); //TODO
			//lifter.poo();
			break;
		case eActCmd_AC_LIFTER_POOENDED:
			lifter.pooEnded();
			break;
	/**
	 * ARMS
	 */
		case eActCmd_AC_ARMS_START:
			arms.start();
			break;
		case eActCmd_AC_ARMS_STOP:
			arms.stop();
			break;
		case eActCmd_AC_ARMS_SWALLOW:
			arms.swallowNormal();
			break;
		case eActCmd_AC_ARMS_FASTPOO:
			arms.fastPoo(0);
			break;
		case eActCmd_AC_ARMS_POO:
			NOT_IMPLEMENTED(); //TODO
			//arms.poo();
			break;
		case eActCmd_AC_ARMS_RETRACT:
			arms.retract();
			break;
		case eActCmd_AC_ARMS_ROT_ENGAGE:
			NOT_IMPLEMENTED(); //TODO
			break;
		case eActCmd_AC_ARMS_ROT_RETRACT:
			NOT_IMPLEMENTED(); //TODO
			break;

	/**
	 * FUNNY
	 */
		case eActCmd_AC_FUNNY_START:
			servoFunnyAction.goTo(FUNNY_REST);
			break;
		case eActCmd_AC_FUNNY_STOP:
			servoFunnyAction.disable();
			break;
		case eActCmd_AC_FUNNY_LAUNCH:
			servoFunnyAction.goTo(FUNNY_LAUNCH);
			break;

	/**
	 * default
	 */
		default:
			ASSERT(false);
			break;

	}
}

void ActuatorThread::turnWheels(eWheelsCmd on)
{
    switch(on)
    {
    case WC_IDLE:
        servoLeftWheel.goTo(480);
        servoRightWheel.goTo(480);
        break;

    case WC_SWALLOW:
        servoLeftWheel.goTo(0);
        servoRightWheel.goTo(1000);
        break;

    case WC_WITHDRAW:
        servoLeftWheel.goTo(1000);
        servoRightWheel.goTo(0);
        break;
    }
}

void ActuatorThread::lifterCmd(bool up)
{
    if(up)
    {
        servoLifter.goTo(360);
    }
    else
    {
        servoLifter.goTo(790);
    }
}



