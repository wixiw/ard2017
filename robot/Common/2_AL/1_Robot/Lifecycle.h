/*
 * StrategyThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_LIFECYCLE_H_
#define ROBOTS_LIFECYCLE_H_

#include "../1_Robot/IStrategy.h"
#include "../1_Robot/YakardTimer.h"
#include "RSP.h"

#define private public //workaround to gain instropection
#include "generated/FSM_Lifecycle.h"
#undef private

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

    /**
     * This class is used to create the instrospection that Yakindu refuses to do
     */
    class FSM_Lifecycle_Better: public FSM_Lifecycle
    {
    public:
        FSM_Lifecycle_Better();
        void run();
        FSM_LifecycleStates getState() const
        {
            return stateConfVector[0];
        };
        String state2Str(FSM_LifecycleStates state) const;
    protected:
        FSM_LifecycleStates lastState;
    };

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

        //The match is finished, listener is supposed to stop any actuator that is not involved in funny action
        virtual void matchEnded() = 0;
    };

    /**
     * This class manages the robot highest functionnal level. The global state machine named Lifecycle
     * manages robots "modes". In a mode some actions are allowed and other are not
     */
    class Lifecycle: public Thread, public FSM_Lifecycle::DefaultSCI_OCB
    {
    public:
        Lifecycle(Navigation& nav, Chrono& chrono, HmiThread& hmi);

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

        //display preamble information to the user
        //it's just a mean to reduce the volume of code in the run function
        //TODO WIX : mettre a jour dans la machine et update commentaire
        void displayStrategies();

        /**
         * -----------------------------
         * FSM interface
         */

        //automatic robot configuration (network command from vizu)
        void networkConfigRequest(uint8_t strategyId_, eColor matchColor);

        //automatic match start (network command from vizu)
        void startMatch();

        //robot strategy may request to end the match sooner than expected
        void endStrategy();

        //robot strategy may request to end the funny action sooner than expected
        void endFunnyAction();

        //FSM callback : infor listener that robot has finished it's boot
        //(the strategy strat == lifecycle is the less priority thread so when it runs
        //for the first time, it means that anything else has finished booting.
        void bootUp() override;

        //FSM callback
        void beep(sc_integer nb) override;

        //FSM event timer
        YakardTimer fsmTimer;

    protected:
        //Updates FSM inputs
        void readInputs();

        //Take into account FSM outputs
        void publishOutputs();

        //force the match configuration without using the HMI
        void configureMatch(uint8_t strategyId, eColor matchColor);

        //a value identifying the strategy choosed by the user
        uint8_t strategyId;

        //list of strategies
        MatchDescriptor matchs[NB_MAX_STRATEGIES];
        uint8_t nbRegisteredStrats;

        //State machine behavior
        FSM_Lifecycle_Better fsm;

        //References to dependencies
        LifecycleListener* listener;
        Navigation& nav;
        Chrono& chrono;
        HmiThread& hmi;

    };

} /* namespace ard */

#endif /* ROBOTS_LIFECYCLE_H_ */
