/*
 * ActuatorThread.cpp
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#include "ActuatorThread.h"
#include "Motion/KinematicManager.h"

using namespace ard;

ActuatorThread::ActuatorThread(KinematicManager& kinMan, HmiThread& hmi):
        PollerThread("Actuator", PRIO_ACTUATORS, STACK_ACTUATORS, PERIOD_ACTUATORS, 11),
        stockColor(),
        switchArmLout(      SWITCH_ARM_L_OUT, 100, 10, true),
        switchArmLin(       SWITCH_ARM_L_IN, 100, 10, true),
        switchArmRout(      SWITCH_ARM_R_OUT, 100, 10, true),
        switchArmRin(       SWITCH_ARM_R_IN, 100, 10, true),
        omronCylinder(      OMRON2, 200, 50, true),
        switchLifterUp(     SWITCH_LIFTER_UP, 100, 10, true),
		switchLifterDown(GPIO_LOW),
        //TODO switchLifterDown(   SWITCH_LIFTER_DOWN, 100, 10, true),
        servoLifter("Lifter", 			SERVO1, LIFTER_MIN, LIFTER_MAX),
        servoLeftArm("LeftArm", 		SERVO2, ARM_MIN, ARM_MAX),
        servoRightArm("RightArm", 		SERVO3, ARM_MIN, ARM_MAX, true),
        servoLeftWheel("LeftWheel",     SERVO4, 0, 1000),
        servoRightWheel("RightWheel",   SERVO5, 0, 1000, true),
        servoFunnyAction("Funny", 		SERVO6, 0, 1000),
		servoRotator("Rotateur", 		SERVO7, 0, 1000),
		servo8("Servo8", 				SERVO8, 0, 1000),
		rotatorMotor(MOSFET1),
        lifter(*this, fsmTimeWheel, hmi),
        arms(*this, fsmTimeWheel, hmi),
        faceUp(*this),
		state(),
		fsmTimeWheel(),
		kinematics(kinMan),
		hmi(hmi)
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

    if( arms.isReady() )
    	hmi.led3.set(ON);
    else
    	hmi.led3.set(OFF);
    if( lifter.isReady() )
    	hmi.led4.set(ON);
    else
    	hmi.led4.set(OFF);
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
    state.lifterTimeout		= lifter.isInTimeout();
    state.lifterError		= lifter.isBlocked();

    state.armsReady			= arms.isReady();
    state.armsTimeout 		= arms.isInTimeout();
    state.armsError			= arms.isBlocked();

    return state;
}

void ActuatorThread::startAll()
{
	//c'est fait avec les bras lifter.start();
	arms.start();
}

void ActuatorThread::stopAll()
{
	//c'est fait avec les bras lifter.stop();
	arms.stop();
	disableAll();
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
	LOG_DEBUG("Actuator command received : " + String((int)cmd));

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
		case eActCmd_AC_LIFTER_PREPARE_NEXT_TO_POO:
			lifter.prepareNextToPoo();
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
			arms.fastPoo();
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

		case eActCmd_AC_ARMS_DISPENSER:
			arms.swallowDispenser();
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


