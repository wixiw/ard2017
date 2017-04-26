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
}

void Arms::init()
{
    ASSERT(lifter!=NULL);
    fsm.init();
    fsm.enter();
}

void Arms::start()
{
    NOT_IMPLEMENTED(); //TODO : corriger les valeurs des servos avant d'utiliser
    fsm.set_start(true);
}

void Arms::update(TimeMs sinceLastCall)
{
    fsm.set_leftExtendedSwitch(acts.switchArmLout.read());
    fsm.set_rightExtendedSwitch(acts.switchArmRout.read());
    fsm.set_leftRetractedSwitch(acts.switchArmLin.read());
    fsm.set_rightRetractedSwitch(acts.switchArmRin.read());

    fsm.getSCI_Strategy()->set_lifterReady(lifter->isReadyForNextCylinder());

    fsm.runCycle();

    if( fsm.get_start() )
    {
        acts.servoLeftArm.      write(fsm.get_leftArm());
        acts.servoRightArm.     write(fsm.get_rightArm());
        acts.servoLeftWheel.    write(fsm.get_leftWheel());
        acts.servoRightWheel.   write(fsm.get_rightWheel());
    }
}

void ard::Arms::swallow(uint8_t nbCylinders)
{
    NOT_IMPLEMENTED(); //TODO : corriger les valeurs des servos avant d'utiliser
}

void ard::Arms::retractArms()
{
    NOT_IMPLEMENTED(); //TODO : corriger les valeurs des servos avant d'utiliser
}

void ard::Arms::withdraw(uint8_t nbCylinders)
{
    NOT_IMPLEMENTED(); //TODO : corriger les valeurs des servos avant d'utiliser
}

void ard::Arms::poo(uint8_t nbCylinders)
{
    NOT_IMPLEMENTED(); //TODO : corriger les valeurs des servos avant d'utiliser
}

