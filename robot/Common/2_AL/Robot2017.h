/*
 * Robot2017.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ROBOT2017_H_
#define ROBOTS_ROBOT2017_H_

#include "BSP.h"
#include "RSP.h"
#include "ActuatorThread.h"
#include "ActuatorX.h"
#include "StrategyThread.h"
#include "TeleopThread.h"

#define ROBOT Robot2017::getInstance()

namespace ard
{
    /**
     * This class assemble all the robot SW elements into a system
     * It aims at being the unique global var so that var creation order is managed
     * by this class constructor.
     *
     * Most members are public as this class aims at gathering all object references
     * It would be a pain (and stupid) to catch all them with non-const getter
     */
    class Robot2017
    {
    public:
        //-------------------------------------------------------------------
        // start of Strategy API
        //-------------------------------------------------------------------

        //Freeze the robot so we are sure it doesn't do
        //anything until the end of the match
        //Note : it will still do the funny action
        void dieMotherFucker();

        //answer true if the start is plugged
        bool isStartPlugged();

        //Retrieve the event published when the start is inserted
        IEvent* getStartInEvt();

        //Retrieve the event published when the start is withdrawn
        IEvent* getStartOutEvt();

        //answer true when color switch is on the preferred color
        bool isPreferedColor();

        //Combines strategy switch to creates an integer. Left switch represent highest bit, Right one lowest one.
        uint8_t getStrategyId();

        //Drive the RGB LED
        void setRGBled(eRgb color, eLedState blink);

        //Drive the 4 green leds
        //for param led see BSH.h : LED1, LED2, LED3, LED4
        void setLed(uint8_t led, eLedState blink);

        //Retrive any of the teleop events
        IEvent* getTeleopEvt(eTeleopEvtId id);

        //-------------------------------------------------------------------
        // End of Strategy API
        //-------------------------------------------------------------------

        //retrieve the singleton instance (you should prefer the use of the g_ArdOs maccro)
        static Robot2017&
        getInstance()
        {
            return instance;
        }
        ;

        //Initialize instances and start the robot
        //This function never ends
        void boot();

        //Applicative layer
        ActuatorThread actuators;
        StrategyThread strategy;
        ActuatorX claws;

        //Public RSP interface : because i'm too lazy to hide it
        Navigation nav;

    private:
        //RSP implementation
        HmiThread hmi;
        LogThread& log;
        TeleopThread teleop;

        //singleton instance
        static Robot2017 instance;

        //Assemble all object instances
        //private constructor as its a singleton class
        Robot2017();
        Robot2017& operator=(const Robot2017& p);
        Robot2017(const Robot2017& p);

    };

} /* namespace ard */

#endif /* ROBOTS_ROBOT2017_H_ */
