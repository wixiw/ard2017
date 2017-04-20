/*
 * ActuatorThread.h
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ACTUATORTHREAD_H_
#define ROBOTS_ACTUATORTHREAD_H_

#include "StratFwk.h"

#ifdef BUILD_STRATEGY

#include "Arms.h"
#include "Lifter.h"

namespace ard
{
    class ActuatorThread: public PollerThread
    {
    public:
        ActuatorThread();

        //Overrides Thread : register polled objects
        void init() override;

        //Overrides Thread : run timer for state machines
        void run() override;

        //used for telemetry or any instropection
        apb_ActuatorsState const& getState();

        //data are public to prevent having to write a decorator, but anyone is welcome to do it
        ColorSensor stockColor;

        FilteredInput switchArmLout;
        FilteredInput switchArmLin;
        FilteredInput switchArmRout;
        FilteredInput switchArmRin;
        FilteredInput omronCylinder;
        FilteredInput switchCylinder;
        FilteredInput omronSpare;
        FilteredInput switchLifterUp;
        FilteredInput switchLifterDown;

        Servo         servoLifter;
        Servo         servoLeftArm;
        Servo         servoRightArm;
        Servo         servoLeftWheel;
        Servo         servoRightWheel;
        Servo         servoFunnyAction;

    private:
        apb_ActuatorsState state; //cache to hold telemetry data

        YakardTimer fsmTimeWheel;
        Lifter      lifter;
        Arms        arms;
    };

} /* namespace ard */

#endif //BUILD_STRATEGY

#endif /* ROBOTS_ACTUATORTHREAD_H_ */
