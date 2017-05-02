/*
 * StrategyThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "Lifecycle.h"
#include "Robot2017.h"

using namespace ard;

FSM_Lifecycle_Better::FSM_Lifecycle_Better():
        lastState(FSM_Lifecycle_last_state)
{

}

void FSM_Lifecycle_Better::run()
{
    runCycle();
    FSM_LifecycleStates newState = getState();
    if( lastState != newState)
    {
        LOG_INFO(String("[Lifecycle] state changed :  ") + state2Str(lastState) + " => state " + state2Str(newState));
        lastState = newState;
    }
}

String FSM_Lifecycle_Better::state2Str(FSM_LifecycleStates state) const
{
    switch(state)
    {
        case main_region_Waiting_table_installation:
            return "Waiting_table_installation";
            break;
        case main_region_Table_installation:
            return "Table_installation";
            break;
        case main_region_Waiting_match:
            return "Waiting_match";
            break;
        case main_region_Match:
            return "Match";
            break;
        case main_region_Waiting_match_end:
            return "Waiting_match_end";
            break;
        case main_region_Funny_action:
            return "Funny_action";
            break;
        case main_region_Finished:
            return "Finished";
            break;
        case main_region_Waiting_start:
            return "Waiting_start";
            break;
        case main_region_Booting:
            return "Booting";
            break;
        case FSM_Lifecycle_last_state:
            return "init state";
            break;
        default:
            return String("unknown state (") + state + ")";
            break;
    }
}

Lifecycle::Lifecycle(Navigation& nav, Chrono& chrono, HmiThread& hmi):
        Thread("Strategy", PRIO_STRATEGY, STACK_STRATEGY, PERIOD_STRATEGY),
        strategyId(0),
        nbRegisteredStrats(0),
        listener(NULL),
        nav(nav),
        chrono(chrono),
        hmi(hmi)
{
    fsm.setDefaultSCI_OCB(this);
    fsm.setTimer(&fsmTimer);
    INIT_TABLE_TO_ZERO(matchs);
}

//Dependency injection
void Lifecycle::attach(LifecycleListener* client)
{
    ASSERT_TEXT(client,"You should not attach a NULL robot");
    listener = client;
}

void Lifecycle::init()
{
    ASSERT(nbRegisteredStrats);
    Thread::init();
    fsm.init();
    fsm.enter();
}

void Lifecycle::run()
{
    fsmTimer.run(PERIOD_STRATEGY);
    readInputs();
    fsm.run();
    publishOutputs();
}

void Lifecycle::registerMatchType(String const& name, IStrategy* install, IStrategy* match, IStrategy* funny)
{
    ASSERT_TEXT(name != "", "Strat name shall not be empty");
    ASSERT_TEXT(nbRegisteredStrats < NB_MAX_STRATEGIES, "Too many matchs registered.");
    matchs[nbRegisteredStrats].name     = name;
    matchs[nbRegisteredStrats].install  = install;
    matchs[nbRegisteredStrats].match    = match;
    matchs[nbRegisteredStrats].funny    = funny;
    matchs[nbRegisteredStrats].linear    = NULL;
    nbRegisteredStrats++;
}

void Lifecycle::registerLinearStrat(String const& name, StrategyFunctor functor)
{
    ASSERT_TEXT(name != "", "Strat name shall not be empty");
    ASSERT_TEXT(nbRegisteredStrats < NB_MAX_STRATEGIES, "Too many matchs registered.");
    matchs[nbRegisteredStrats].name     = name;
    matchs[nbRegisteredStrats].install  = NULL;
    matchs[nbRegisteredStrats].match    = NULL;
    matchs[nbRegisteredStrats].funny    = NULL;
    matchs[nbRegisteredStrats].linear  = functor;
    nbRegisteredStrats++;
}

void Lifecycle::networkConfigRequest(uint8_t strategyId_, eColor matchColor)
{
    configureMatch(strategyId_, matchColor);
    fsm.raise_networkConfigRequest();
}

void Lifecycle::startMatch()
{
    fsm.raise_startMatch();
}

void Lifecycle::endStrategy()
{
    fsm.raise_strategyFinished();
}

void Lifecycle::endFunnyAction()
{
    fsm.raise_funnyActionFinished();
}

void Lifecycle::readInputs()
{
    fsm.set_startIn                 (hmi.isStartPlugged());
    fsm.set_strategyRemainingTime   (chrono.getStrategyRemainingTime());
    fsm.set_matchDuration           (chrono.getTime());
}

void Lifecycle::publishOutputs()
{
    if( fsm.isRaised_installStarted() )
    {
        //Read color input
        eColor selectedColor = eColor_UNKNOWN;
        if ( hmi.isColorSwitchOnPrefered() )
            selectedColor = eColor_PREF;
        else
            selectedColor = eColor_SYM;

        //Read strat config
        configureMatch(hmi.getStrategyId(), selectedColor);

        matchs[strategyId].install->start();
    }

    if( fsm.isRaised_matchStarted() )
    {
        //avoidance system is only activated with start
        if( !fsm.get_networkStart() )
            nav.enableAvoidance(true);

        //start coutning match time
        chrono.startMatch();

        //starts the machine
        if( matchs[strategyId].match != NULL)
            matchs[strategyId].match->start();
    }

    if( fsm.isRaised_funnyStarted() )
    {
        matchs[strategyId].funny->start();
    }

    if( fsm.isRaised_matchEnded() )
    {
        if(listener)
            listener->matchEnded();
    }

    //Robot installation strategy
    if( fsm.isStateActive(FSM_Lifecycle::main_region_Table_installation) && matchs[strategyId].install != NULL)
        matchs[strategyId].install->update(PERIOD_STRATEGY);

    //Match running
    if( fsm.isStateActive(FSM_Lifecycle::main_region_Match))
    {
        if( matchs[strategyId].linear != NULL)
            matchs[strategyId].linear(*reinterpret_cast<Robot2017*>(listener) /* #porky */);

        if( matchs[strategyId].match != NULL)
            matchs[strategyId].match->update(PERIOD_STRATEGY);
    }

    //Funny action
    if( fsm.isStateActive(FSM_Lifecycle::main_region_Funny_action) && matchs[strategyId].funny != NULL )
        matchs[strategyId].funny->update(PERIOD_STRATEGY);
}

void Lifecycle::bootUp()
{
    if( listener )
        listener->bootUp();
    LOG_INFO(String("Robot is booted successfully, it took ") + millis() + " ms.");
}

void Lifecycle::displayStrategies()
{
    LOG_INFO("Available matchs : ");
    for (int i = 0; i < NB_MAX_STRATEGIES; ++i)
    {
        if( matchs[i].name != "" )
        {
            sleepMs(10);//Let Log thread do its job
            LOG_INFO("    [" + String(i) + "]: " + matchs[i].name);
        }
    }
}

void Lifecycle::beep(sc_integer nb)
{
    hmi.buzzer.bip(nb);
}

void Lifecycle::configureMatch(uint8_t strategyId_, eColor matchColor)
{
    //Check selected strategy
    if( nbRegisteredStrats <= strategyId_)
    {
        LOG_ERROR(String("Strategy ") +strategyId_+ " is unknown, using strat 0 instead.");
        strategyId_ = 0;
    }

    ASSERT_TEXT(matchs[strategyId].name != "", "Selected strategy is malformed.");
    strategyId = strategyId_;
    LOG_INFO(String("User has selected strategy [") + strategyId_ + "] " + matchs[strategyId].name + ".");

    //Configure color
    if(listener)
        listener->colorChoosed(matchColor);
}
