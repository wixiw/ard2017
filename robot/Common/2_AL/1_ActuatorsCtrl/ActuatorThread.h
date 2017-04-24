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

        /**---------------------------------
         * Strat API
         ---------------------------------*/
        //Open arm and make wheel turn to swallow several cylinder on table
        void swallow(uint8_t nbCylinders);

        //Stop turning wheels and retract arms to be able to move safely on table
        void retractArms();

        //Take a certain number of cylinders from the dispenser
        void withdraw(uint8_t nbCylinders);

        //Poo a certain number of cylinder (it doesn't matter if it's on table or in a container)
        void poo(uint8_t nbCylinders);

        //disable all actuators except those for used in funny actionn (typically used at end of match)
        void disableAll();

        /**---------------------------------
         * Container thread interface
         ---------------------------------*/

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
