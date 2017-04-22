/*
 * Arms.cpp
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#include "Arms.h"
#include "ActuatorThread.h"

#ifdef BUILD_STRATEGY

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
}

void Arms::start()
{
    fsm.enter();
    fsm.getDefaultSCI()->raise_start();
}

void Arms::update(TimeMs sinceLastCall)
{
    fsm.getDefaultSCI()->set_leftExtendedSwitch(acts.switchArmLout.read());
    fsm.getDefaultSCI()->set_rightExtendedSwitch(acts.switchArmRout.read());
    fsm.getDefaultSCI()->set_leftRetractedSwitch(acts.switchArmLin.read());
    fsm.getDefaultSCI()->set_rightRetractedSwitch(acts.switchArmRin.read());

    fsm.getSCI_Strategy()->set_lifterReady(lifter->isReadyForNextCylinder());

    fsm.runCycle();

    //TODO convert 0=>100% to 0=>180
//    acts.servoLeftArm.      write(fsm.getDefaultSCI()->get_leftArm());
//    acts.servoRightArm.     write(fsm.getDefaultSCI()->get_rightArm());
//    acts.servoLeftWheel.    write(fsm.getDefaultSCI()->get_leftWheel());
//    acts.servoRightWheel.   write(fsm.getDefaultSCI()->get_rightWheel());
}

#endif
