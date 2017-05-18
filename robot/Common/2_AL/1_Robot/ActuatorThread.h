/*
 * ActuatorThread.h
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ACTUATORTHREAD_H_
#define ROBOTS_ACTUATORTHREAD_H_

#include "Com.h"
#include "RSP.h"
#include "Arms.h"
#include "FaceUp.h"
#include "Lifter.h"


namespace ard
{
    typedef enum
    {
        WC_IDLE = 0,
        WC_SWALLOW = 1,
        WC_WITHDRAW = -1
    } eWheelsCmd;

    class ActuatorThread: public PollerThread
    {
    public:
        ActuatorThread();

        /**---------------------------------
         * Strat API
         ---------------------------------*/
        //Open arm and make wheel turn to swallow several cylinder on table
        void swallow(bool on);

        //1 : swallow, -1 : expusle, 0 : stops
        void turnWheels(eWheelsCmd on);

        //Rise lifter when true, lower if false
        void lifterCmd(bool up);

        //Cylinder rotation cmd
        void turnCylinder(bool turn);

        //Turn the cylinder
        void faceUpCylinder();

        //Return the state of faceUpCylinderCmd()
        eFaceUpStatus getFaceUpStatus();

        //disable all actuators except those for used in funny actionn (typically used at end of match)
        void disableAll();

        /**---------------------------------
         * Container thread interface
         ---------------------------------*/

        //Overrides Thread : register polled objects
        void init();

        //Overrides Thread : run timer for state machines
        void run();

        //used for telemetry or any instropection
        apb_ActuatorsState const& serealize();

        //Selects the match color
        void setColor(eColor color);

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

        HomingServo    servoLifter;
        HomingServo    servoLeftArm;
        HomingServo    servoRightArm;
        Servo         servoLeftWheel;
        Servo         servoRightWheel;
        Servo         servoFunnyAction;

    private:
        apb_ActuatorsState state; //cache to hold telemetry data

        YakardTimer   fsmTimeWheel;
        Lifter        lifter;
        Arms          arms;
        FaceUp        faceUp;
    };

} /* namespace ard */

#endif /* ROBOTS_ACTUATORTHREAD_H_ */
