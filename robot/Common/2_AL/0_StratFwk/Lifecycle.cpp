/*
 * StrategyThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "Lifecycle.h"
#include "Robot2017.h"

#ifdef BUILD_STRATEGY

using namespace ard;
extern String getExeVersion();

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
        case main_region_Match_ended:
            return "Match_ended";
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
            ASSERT(false);
            return "";
            break;
    }
}

Lifecycle::Lifecycle(Robot2017* newRobot):
        Thread("Strategy", PRIO_STRATEGY, STACK_STRATEGY, PERIOD_STRATEGY),
        strategyId(0),
        nbRegisteredStrats(0),
        robot(newRobot)
{
    fsm.setDefaultSCI_OCB(this);
    INIT_TABLE_TO_ZERO(matchs);

    ASSERT_TEXT(newRobot,"You should not attach a NULL robot");
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

void Lifecycle::endMatch()
{
    fsm.raise_strategyFinished();
}

void Lifecycle::endFunnyAction()
{
    fsm.raise_funnyActionFinished();
}

void Lifecycle::readInputs()
{
    fsm.set_startIn                 (robot->isStartPlugged());
    fsm.set_strategyRemainingTime   (robot->chrono.getStrategyRemainingTime());
    fsm.set_matchDuration           (robot->chrono.getTime());
}

void Lifecycle::publishOutputs()
{
    if( fsm.isRaised_readHmiConfig() )
    {
        //Read color input
        eColor selectedColor = eColor_UNKNOWN;
        if ( robot->isColorSwitchOnPrefered() )
            selectedColor = eColor_PREF;
        else
            selectedColor = eColor_SYM;

        //Read strat config
        configureMatch(robot->getStrategyId(), selectedColor);
    }

    if( fsm.isRaised_matchStarted() )
    {
        //avoidance system is only activated with start
        if( !fsm.get_networkStart() )
            robot->nav.enableAvoidance(true);

        //start coutning match time
        robot->chrono.startMatch();
    }

    if( fsm.isStateActive(FSM_Lifecycle::main_region_Table_installation) && matchs[strategyId].install != NULL)
    {
        matchs[strategyId].install->update(PERIOD_STRATEGY);
    }

    if( fsm.isStateActive(FSM_Lifecycle::main_region_Match) && matchs[strategyId].match != NULL )
    {
        matchs[strategyId].match->update(PERIOD_STRATEGY);
    }

    if( fsm.isStateActive(FSM_Lifecycle::main_region_Funny_action) && matchs[strategyId].funny != NULL )
    {
        matchs[strategyId].funny->update(PERIOD_STRATEGY);
    }
}

void Lifecycle::displayIntroduction()
{
    LOG_INFO("--------------------------------------");

    #ifdef ARD_DEBUG
    LOG_INFO(" --- DEBUG --- (see ARD_DEBUG in K_constants.h) ");
    #else
    LOG_INFO("Tips : In order to see debug logs, define ARD_DEBUG in ArdOs.h.");
    #endif

    //Trace binary version to prevent miss build error and usage error during the middle of the night.
    LOG_INFO("Version libArd : " + robot->getVersion());
    LOG_INFO(getExeVersion());

    LOG_INFO(String("Robot is booted successfully, it took ") + millis() + " ms.");
    robot->sendSerialNumber();

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
    robot->buzzer().bip(nb);
}

void Lifecycle::configureMatch(uint8_t strategyId_, eColor matchColor)
{
    robot->nav.setColor (matchColor);

    //Configure color
    if ( matchColor == eColor_PREF )
    {
        robot->setRGBled(YELLOW, ON);
        robot->strategy.setColor(eColor_PREF);
        LOG_INFO("User has selected PREF (Yellow) color.");
    }
    else if ( matchColor == eColor_SYM )
    {
        robot->nav.setColor (eColor_SYM);
        robot->strategy.setColor(eColor_SYM);
        robot->setRGBled(BLUE, ON);
        LOG_INFO("User has selected SYM (Blue) color.");
    }
    else
    {
        ASSERT_TEXT(false, "StrategyThread::configureMatch : color should not be unknown.");
    }

    //Check selected strategy
    ASSERT(strategyId_ < nbRegisteredStrats);
    ASSERT_TEXT(matchs[strategyId].name != "", "Selected strategy is malformed.");
    strategyId = strategyId_;
    LOG_INFO(String("User has selected strategy [") + strategyId_ + "] " + matchs[strategyId].name + ".");
}

#endif
