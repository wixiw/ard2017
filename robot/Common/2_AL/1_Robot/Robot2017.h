/*
 * Robot2017.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_Robot2017_Robot2017_H_
#define ROBOT_Robot2017_Robot2017_H_

#include "ActuatorThread.h"
#include "Lifecycle.h"
#include "StrategyModel2017.h"

//Timer 0 reserved for ARD cpu stats, instanciated for IRQ priority config
#define TIMER_CPU           Timer0
//Timer 1 reserved for the Servo lib, instanciated for IRQ priority config
#define TIMER_SERVO         Timer5

//Timer available for Duetimer lib
#define TIMER_NAV_STEPPER   Timer2
#define TIMER_GPIO          Timer3
#define TIMER_BUZZER        Timer4

namespace ard
{
    class Robot2017Listener
    {
    public:
        virtual ~Robot2017Listener() = default;

        //Robot boot is finished, the robot is ready to operate
        virtual void bootUp() = 0;
    };


    /**
     * This class assemble all the robot SW elements into a system
     * It aims at being the unique global var so that var creation order is managed
     * by this class constructor.
     *
     * Most members are public as this class aims at gathering all object references
     * It would be a pain (and stupid) to catch all them with non-const getter
     */
    class Robot2017 : public LifecycleListener
    {
    public:
        Robot2017();
        virtual ~Robot2017() = default;
        
        //-------------------------------------------------------------------
        // Wiring
        //-------------------------------------------------------------------

        //Initialize the robot
        virtual void init(Robot2017Listener* client);

        //Start the OS, This function never ends
        void boot();

        //Utility function to get version info
        String const& getVersion(){return buildDate;};

        //For some reason Atmel is shitting with build dependencies, here is a way to have the exe build date
        //It's supposed to be close to Library date
        virtual String const& getExeVersion() = 0;

        //Get a reference to an LSA
        virtual LSA& getLSA(uint8_t id) const = 0;

        //Get the FSM event timer
        YakardTimer& getFsmTimer();

        //Stop all functionnal threads and keep debugging thread working.
        void SWAssert();

        //-------------------------------------------------------------------
        // Lifecycle listener
        //-------------------------------------------------------------------

        //Display introduction text
        void bootUp() override;

        //Configure the robot with the desired color
        void colorChoosed(eColor color) override;

        //Cut any moving actuator/navigation except funny action
        void coreMatchEnded() override;

        //Cut any moving actuator/navigation
        void funnyActionEnded() override;

        //-------------------------------------------------------------------
        // start of Strategy API
        //-------------------------------------------------------------------

        //Freeze the robot so we are sure it doesn't do
        //anything until the end of the match
        virtual void dieMotherFucker();

        //Get the robot parameters
        RobotParameters const& params() const {return m_params;};

        //-------------------------------------------------------------------
        // End of Strategy API
        //-------------------------------------------------------------------

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

        //required to map efficiently HW interrupts
        static Robot2017* instance;

        //hardware layer
        BSP bsp;

        LogDispatcher& log;

    protected:
        SdCardLogger fileLogger;

        //Save the ARD library build date in the binary
        String buildDate;

        //Robot parameters
        RobotParameters m_params;

    public:
        //RSP implementation
        HmiThread hmi;

        //Public RSP interface : because i'm too lazy to hide it, please feel free to implement the decorator
        Navigation nav;

        //Chrono keeps track of the time during the match
        Chrono chrono;

        //Models the environment state
        StrategyModel2017 stratInfo;

        //Drive actuators
        ActuatorThread actuators;

        //StrategyRunner
        Lifecycle lifecycle;

    private:
        Robot2017Listener* listener;

        //delete some unexpected copy
        Robot2017& operator=(const Robot2017& p) = delete;
        Robot2017(const Robot2017& p) = delete;
    };
} /* namespace ard */



#endif /* ROBOT_Robot2017_Robot2017_H_ */
