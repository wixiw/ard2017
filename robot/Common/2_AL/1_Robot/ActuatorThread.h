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

    //forward declare
    class KinematicManager;

    class ActuatorThread: public PollerThread, public RobotParametersListener
    {
    public:
        ActuatorThread(KinematicManager& kinMan);

        /**---------------------------------
         * Strat API
         ---------------------------------*/
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

        //Initiate all actuators systems
        void startAll();

        //disable all actuators except those for used in funny actionn (typically used at end of match)
        void disableAll();

        //Simplified API
        void actCmd(eActCmd cmd);

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
        FilteredInput switchLifterUp;
        FilteredInput switchLifterDown;

        AccelServo    servoLifter;
        AccelServo    servoLeftArm;
        AccelServo    servoRightArm;
        AccelServo    servoLeftWheel;
        AccelServo    servoRightWheel;
        AccelServo    servoFunnyAction;
        AccelServo    servoRotator;
        AccelServo    servo8;

        Lifter        lifter;
        Arms          arms;
        FaceUp        faceUp;

    private:
        apb_ActuatorsState state; //cache to hold telemetry data

        YakardTimer   fsmTimeWheel;

        KinematicManager& kinematics;
    };

} /* namespace ard */

#endif /* ROBOTS_ACTUATORTHREAD_H_ */
