/*
 * StrategyThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "StrategyThread.h"
#include "Robot2017.h"

using namespace ard;

StrategyThread::StrategyThread()
        : strategyId(0)
{
    INIT_TABLE_TO_ZERO(strategies);
}

void StrategyThread::init()
{
    //create the thread
    g_ArdOs.createThread_Cpp("Strategy", *this, STACK_STRATEGY, PRIO_STRATEGY);

    //init the event mailbox
    EventListener::init<1>();
}

void StrategyThread::run()
{
    auto evt_startIn = ROBOT.getStartInEvt();
    auto evt_startOut = ROBOT.getStartOutEvt();
    auto evt_teleopConfigure = ROBOT.getTeleopEvt(EVT_CONFIGURE);
    auto evt_teleopStart = ROBOT.getTeleopEvt(EVT_START_MATCH);

    for (int i = 0; i < NB_MAX_STRATEGIES; ++i)
    {
        LOG(INFO, "Strategy[" + String(i) + "]: " + strategies[i].name);
    }

    //wait for start insertion or teleop command
    {
        LOG(INFO, "STRAT : Waiting strategy/color configuration and start insertion.");
        IEvent* evts[] =
        {   evt_startIn, evt_teleopConfigure};
        auto triggeredEvent = waitEvents(evts, 2);

        //In case the start is inserted, read the HMI switches to configure the strat
        if( triggeredEvent == evt_startIn )
        {
            readUserInputs();
        }
        //else : if the teleop command is received, there is nothing to do as Teleop has already configured ourself with configureMatch
    }

    //wait for start withdraw or a teleop command to start the match
    {
        LOG(INFO, "STRAT : Waiting start withdraw to begin strategy.");
        IEvent* evts[] =
        {   evt_startOut, evt_teleopStart};
        waitEvents(evts, 2); //returned event is not read as we don't care, result will be the same

        //Execute selected strategy
        strategies[strategyId].functor();
    }
}

void StrategyThread::registerStrategy(String name, StrategyFunctor functor)
{
    static uint8_t nbRegisteredStrats = 0;
    ardAssert(nbRegisteredStrats < NB_MAX_STRATEGIES, "Too many strategies registered.");
    strategies[nbRegisteredStrats].name = name;
    strategies[nbRegisteredStrats].functor = functor;
    nbRegisteredStrats++;
}

void StrategyThread::readUserInputs()
{   
    //Read color input
    eColor selectedColor = eColor_UNKNOWN;
    if ( ROBOT.isPreferedColor() )
        selectedColor = eColor_PREF;
    else
        selectedColor = eColor_SYM;

    //Read strat config
    configureMatch(ROBOT.getStrategyId(), selectedColor);
}

void StrategyThread::configureMatch(uint8_t strategyId_, eColor matchColor)
{
    ROBOT.nav.setColor (matchColor);
    
    //Configure color
    if ( matchColor == eColor_PREF )
    {
        ROBOT.setRGBled(YELLOW, ON);
        LOG(INFO, "User has selected PREF (Yellow) color");
    }
    else if ( matchColor == eColor_SYM )
    {
        ROBOT.nav.setColor (eColor_SYM);
        ROBOT.setRGBled(BLUE, ON);
        LOG(INFO, "User has selected SYM (Blue) color");
    }
    else
    {
        ardAssert(false, "StrategyThread::configureMatch : color should not be unknown");
    }

    //Check selected strategy
    strategyId = strategyId_;
    ardAssert(strategies[strategyId].functor != 0, "Selected strategy functor is null.");
    LOG(INFO, "User has selected strategy " + strategies[strategyId].name);
}

