/*
 * StrategyThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_LIFECYCLE_H_
#define ROBOTS_LIFECYCLE_H_

#include "IStrategy.h"
#include "RSP.h"
#include "generated/FSM_Lifecycle.h"

#define NB_MAX_STRATEGIES 15

namespace ard
{
    class Robot2017; //forward declare

    //Every strategy function shall have this signature :
    typedef void (*StrategyFunctor)(Robot2017& robot);

    //Used to display the list of existing strategies
    struct MatchDescriptor
    {
        String name;
        IStrategy* install;
        IStrategy* match;
        IStrategy* funny;
        StrategyFunctor linear;
    };

    //Due to Yakindu generator issues (const tag missing on getters), we have to duplicate the enum
    typedef enum
    {
        MODE_NONE = 0,
        MODE_SELFTEST,
        MODE_INSTALL,
        MODE_CORE_MATCH,
        MODE_FUNNY_ACTION
    } eMode;

    /**
     * To be implemented by the assembly that builds the robot
     */
    class LifecycleListener
    {
    public:
        virtual ~LifecycleListener() = default;

        //Indicates to the listener that the robot boot is finished
        virtual void bootUp() = 0;

        //Indicates to the listener that the user has chossed a color
        virtual void colorChoosed(eColor color) = 0;

        //The core match is finished, listener is supposed to stop any actuator that is not involved in funny action
        virtual void coreMatchEnded() = 0;

        //The funny action is finished, robot shall stop doing anything
        virtual void funnyActionEnded() = 0;
    };

    /**
     * This class manages the robot highest functionnal level. The global state machine named Lifecycle
     * manages robots "modes". In a mode some actions are allowed and other are not
     */
    class Lifecycle: public Thread, public FSM_Lifecycle::DefaultSCI_OCB
    {
    public:
        Lifecycle(Navigation& nav, Chrono& chrono, HmiThread& hmi, OppDetection& detection);

        /**
         * -----------------------------
         */
        void attach(LifecycleListener* client);

        //Override Thread : create the event
        void init() override;

        //Implements Thread : executes the stategy
        void run() override;

        //register new match in the list
        //@param : the name of the strategy, shall not be empty
        //@param install : the strategy to run before the match to prepare the robot, or NULL to skip
        //@param match : the strategy to run during the match, or NULL to skip
        //@param funny : the funny action to run at the match end, or NULL to skip
        void registerMatchType(String const& name, IStrategy* install, IStrategy* match, IStrategy* funny);

        //register a new simple linear (not periodic) strategy.
        //For backward compatibility with previous design
        void registerLinearStrat(String const& name, StrategyFunctor functor);

        //Register the selftest strategy
        void registerSelftest(IStrategy* selftest);

        //display preamble information to the user
        //it's just a mean to reduce the volume of code in the run function
        void displayStrategies();
        
        //automatic robot configuration (network command from vizu)
        void networkConfigRequest(uint8_t strategyId_, eColor matchColor, bool simulated);

        //automatic match start (network command from vizu)
        void networkStartRequest();
        
        /**
         * -----------------------------
         * FSM interface
         */
        //True if robot has been launched in simulation
        bool isSimulated() override;

        //Manual color and strategy configuration
        void configureColor() override;

        //activates robot avoidance system
        void enableAvoidance() override;

        //FSM callback
        void beep(sc_integer nb) override;

        //FSM callback : infor listener that robot has finished it's boot
        //(the strategy strat == lifecycle is the less priority thread so when it runs
        //for the first time, it means that anything else has finished booting.
        void bootUp() override;

        void logDebug(sc_string msg) override
        {
            LOG_DEBUG(String(msg));
        }

        void logInfo(sc_string msg) override
        {
            LOG_INFO(String(msg));
        }

        void logError(sc_string msg) override
        {
            LOG_ERROR(String(msg));
        }

        /**
         * Get the current mode
         */
        sc_integer getModeStatus() override;

        /**
         * Start a new mode
         */
        void startMode(sc_integer mode) override;

        /**
         * Stop the current mode.
         */
        void stopMode() override;

        //FSM event timer
        YakardTimer fsmTimer;

    protected:
        //Updates FSM inputs
        void readInputs();

        //Take into account FSM outputs
        void publishOutputs();

        //force the match configuration without using the HMI
        void configureMatch(uint8_t strategyId, eColor matchColor, bool simulated);

        //a value identifying the strategy choosed by the user
        uint8_t strategyId;

        //list of strategies
        MatchDescriptor matchs[NB_MAX_STRATEGIES];
        uint8_t nbRegisteredStrats;

        //State machine behavior
        FSM_Lifecycle fsm;

        //References to dependencies
        LifecycleListener* listener;
        IStrategy* selftest;
        Navigation& nav;
        Chrono& chrono;
        HmiThread& hmi;
        OppDetection& detection;

        uint8_t currentMode;
        uint8_t currentModeStatus;

        bool simulated;

    };

} /* namespace ard */

#endif /* ROBOTS_LIFECYCLE_H_ */
