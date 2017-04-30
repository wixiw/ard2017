/*
 * Robot2017.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ROBOT2017_H_
#define ROBOTS_ROBOT2017_H_

#include "BSP.hpp"
#include "RSP.h"

namespace ard
{
    //global var to let interrupts access to the robot instance
    //child class or main shall set the variable.
    class Robot;
    extern Robot* g_robot;

    /**
     * This class assemble all the robot SW elements into a system
     * It aims at being the unique global var so that var creation order is managed
     * by this class constructor.
     *
     * Most members are public as this class aims at gathering all object references
     * It would be a pain (and stupid) to catch all them with non-const getter
     */
    class Robot
    {
    public:
        //Freeze the robot so we are sure it doesn't do
        //anything until the end of the match
        virtual void dieMotherFucker() = 0;

        //Send the robot serial number on communication link
        virtual void sendSerialNumber() = 0;

        //Configure the robot with the desired color
        virtual void setMatchColor(eColor color) = 0;

        //-------------------------------------------------------------------
        // start of Strategy API
        //-------------------------------------------------------------------

        //Get the robot parameters
        RobotParameters const& params() const {return m_params;};

        //answer true if the start is plugged
        bool isStartPlugged() const;

        //answer true when color switch is on the preferred color
        bool isColorSwitchOnPrefered() const;

        //Combines strategy switch to creates an integer. Left switch represent highest bit, Right one lowest one.
        uint8_t getStrategyId() const;

        //Drive the RGB LED
        void setRGBled(eRgb color, eLedState blink);

        //Drive the 4 green leds
        //for param led see BSH.h : LED1, LED2, LED3, LED4
        void setLed(uint8_t led, eLedState blink);

        //buzzer accessor
        Buzzer2017& buzzer(){return hmi.buzzer;};

        apb_HmiState const& getHmiState(){return hmi.getState();};

        //-------------------------------------------------------------------
        // End of Strategy API
        //-------------------------------------------------------------------

        //Initialize instances and start the robot OS
        //This function never ends, you may continue initialization in run()
        void bootOs();

        //Utility function to get version info
        String const& getVersion(){return buildDate;};

        //Get the robot config
        apb_Configuration const& getConfig() const {return m_params.getConfig();};

        //Set the robot config (you cannot do this at any time, think twice before using)
        void setConfig(apb_Configuration const& newConf);

        //retrieve the robot serial nunmber
        char const * const getSerialNumber() const;

        //returns true if the robot is "Pen"
        bool isPen() const;

        //returns true if the robot is "Tration"
        bool isTration() const;

        //hardware layer
        BSP bsp;

        //RSP implementation
        HmiThread hmi;

        //Public RSP interface : because i'm too lazy to hide it, please feel free to implement the decorator
        Navigation nav;

        //Chrono keeps track of the time during the match
        Chrono chrono;

        
    protected:
        LogDispatcher& log;

        SdCardLogger fileLogger;

        //Singleton instance
        static Robot* instance;

        //Save the ARD library build date in the binary
        String buildDate;

        //Robot parameters
        RobotParameters m_params;

        //Assemble all object instances
        //private constructor as its a singleton class
        Robot();
        Robot& operator=(const Robot& p) = delete;
        Robot(const Robot& p) = delete;
        virtual ~Robot() = default;

    };

} /* namespace ard */

#endif /* ROBOTS_ROBOT2017_H_ */
