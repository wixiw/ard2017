/*
 * ActuatorThread.cpp
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#include "ActuatorThread.h"

#ifdef BUILD_STRATEGY

using namespace ard;

ActuatorThread::ActuatorThread():
        PollerThread("Actuator", PRIO_ACTUATORS, STACK_ACTUATORS, PERIOD_ACTUATORS, 3),
        stockColor(),
        switchArmLout(      SWITCH_ARM_L_OUT, 100, 10),
        switchArmLin(       SWITCH_ARM_L_IN, 100, 10),
        switchArmRout(      SWITCH_ARM_R_OUT, 100, 10),
        switchArmRin(       SWITCH_ARM_R_IN, 100, 10),
        omronCylinder(      OMRON_CYLINDER, 200, 200),
        switchCylinder(     SWITCH_CYLINDER, 200, 200),
        omronSpare(         OMRON_SPARE, 50, 50),
        switchLifterUp(     SWITCH_LIFTER_UP, 100, 10),
        switchLifterDown(   SWITCH_LIFTER_DOWN, 100, 10),
        servoLifter(        SERVO1, 0, 180),
        servoLeftArm(       SERVO2, 0, 180),
        servoRightArm(      SERVO3, 0, 180),
        servoLeftWheel(     SERVO4, 0, 180),
        servoRightWheel(    SERVO5, 0, 180),
        servoFunnyAction(   SERVO6, 0, 180),
        fsmTimeWheel(),
        lifter(*this, fsmTimeWheel),
        arms(*this, fsmTimeWheel)
{
    state = apb_ActuatorsState_init_default;
    arms.setLifter(lifter);
//    ASSERT(servoLifter.enable());
//    ASSERT(servoLeftArm.enable());
//    ASSERT(servoRightArm.enable());
//    ASSERT(servoLeftWheel.enable());
//    ASSERT(servoRightWheel.enable());
//    ASSERT(servoFunnyAction.enable());
}

void ActuatorThread::init()
{
    addPolledObject(stockColor);
    addPolledObject(lifter);
    addPolledObject(arms);

    PollerThread::init();
}

void ActuatorThread::run()
{
    fsmTimeWheel.run(getPeriod());
    PollerThread::run();
}

apb_ActuatorsState const& ActuatorThread::getState()
{
    state = apb_ActuatorsState_init_default;

    state.colorSensor       = stockColor.getState();
    state.switchArmLout     = switchArmLout.readRaw();
    state.switchArmLin      = switchArmLout.readRaw();
    state.switchArmRout     = switchArmLout.readRaw();
    state.switchArmRin      = switchArmLout.readRaw();
    state.omronCylinder     = switchArmLout.readRaw();
    state.switchCylinder    = switchArmLout.readRaw();
    state.omronSpare        = switchArmLout.readRaw();
    state.switchLifterUp    = switchArmLout.readRaw();
    state.switchLifterDown  = switchArmLout.readRaw();

    state.servoLifter       = servoLifter.read();
    state.servoLeftArm      = servoLeftArm.read();
    state.servoRightArm     = servoRightArm.read();
    state.servoLeftWheel    = servoLeftWheel.read();
    state.servoRightWheel   = servoRightWheel.read();
    state.servoFunnyAction  = servoFunnyAction.read();


    return state;
}

#endif



