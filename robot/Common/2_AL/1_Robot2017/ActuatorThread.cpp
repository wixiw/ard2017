/*
 * ActuatorThread.cpp
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#include "ActuatorThread.h"

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
        servoLifter(        SERVO1, 0, 1000),
        servoLeftArm(       SERVO2, 0, 1000),
        servoRightArm(      SERVO3, 0, 1000),
        servoLeftWheel(     SERVO4, 0, 1000),
        servoRightWheel(    SERVO5, 0, 1000),
        servoFunnyAction(   SERVO6, 0, 1000),
        fsmTimeWheel(),
        lifter(*this, fsmTimeWheel),
        arms(*this, fsmTimeWheel)
{
    state = apb_ActuatorsState_init_default;
//    ASSERT(servoLifter.enable());
//    ASSERT(servoLeftArm.enable());
//    ASSERT(servoRightArm.enable());
//    ASSERT(servoLeftWheel.enable());
//    ASSERT(servoRightWheel.enable());
//    ASSERT(servoFunnyAction.enable());
}

void ActuatorThread::init()
{
    arms.setLifter(lifter);
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

void ActuatorThread::disableAll()
{
    servoLifter.disable();
    servoLeftArm.disable();
    servoRightArm.disable();
    servoLeftWheel.disable();
    servoRightWheel.disable();
}

void ActuatorThread::swallow(bool on)
{
    NOT_IMPLEMENTED();
}

void ActuatorThread::turnWheels(uint8_t on)
{
    if(on)
    {
        servoLeftWheel.write(0);
        servoRightWheel.write(1000);
    }
    else
    {
        servoLeftWheel.write(480);
        servoRightWheel.write(480);
    }
}

void ActuatorThread::lifterCmd(bool up)
{
    if(up)
    {
        servoLifter.write(750);
    }
    else
    {
        servoLifter.write(345);
    }
}


