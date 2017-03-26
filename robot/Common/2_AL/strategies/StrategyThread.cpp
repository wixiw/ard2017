/*
 * StrategyThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "StrategyThread.h"
#include "../Robot2017.h"

using namespace ard;

StrategyThread::StrategyThread():
        Thread("Strategy", PRIO_STRATEGY, STACK_STRATEGY),
        strategyId(0)
{
    INIT_TABLE_TO_ZERO(strategies);
}

void StrategyThread::init()
{
    Thread::init();

    //init the event mailbox
    EventListener::init<1>();
}

extern String getExeVersion();
void StrategyThread::displayIntroduction()
{
    #ifdef ARD_DEBUG
    LOG_INFO(" --- DEBUG --- (see ARD_DEBUG in K_constants.h) ");
    #else
    LOG_INFO("Tips : In order to see debug logs, define ARD_DEBUG in ArdOs.h.");
    #endif

    //Trace binary version to prevent miss build error and usage error during the middle of the night.
    LOG_INFO("Version libArd : " + ROBOT.getVersion());
    LOG_INFO(getExeVersion());

    LOG_INFO(String("Robot is booted successfully, it took ") + millis() + " ms.");

    LOG_INFO("Available strategies : ");
    for (int i = 0; i < NB_MAX_STRATEGIES; ++i)
    {
        LOG_INFO("    [" + String(i) + "]: " + strategies[i].name);
    }

    ROBOT.setLed(LED1, SLOW_BLINK);
    ROBOT.setLed(LED2, FAST_BLINK);
    ROBOT.setLed(LED3, ON);
    ROBOT.bip(3);
    ROBOT.waitBuzzer();
}

void StrategyThread::run()
{
    auto evt_startIn = ROBOT.getStartInEvt();
    auto evt_startOut = ROBOT.getStartOutEvt();
    auto evt_teleopConfigure = ROBOT.getRemoteControlEvt(EVT_CONFIGURE);
    auto evt_teleopStart = ROBOT.getRemoteControlEvt(EVT_START_MATCH);

    displayIntroduction();

    //wait for start insertion or teleop command
    {
        LOG_INFO("Waiting for user to choose strategy/color and to insert start...");
        IEvent* evts[] =
        {   evt_startIn, evt_teleopConfigure};
        auto triggeredEvent = waitEvents(evts, 2);

        //In case the start is inserted, read the HMI switches to configure the strat
        if( triggeredEvent == evt_startIn )
        {
            readUserInputs();
        }
        //else : if the teleop command is received, there is nothing to do as RemoteControl has already configured ourself with configureMatch
    }

    //wait for start withdraw or a teleop command to start the match
    {
        LOG_INFO("Waiting start withdraw to begin the match...");
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
    ASSERT_TEXT(nbRegisteredStrats < NB_MAX_STRATEGIES, "Too many strategies registered.");
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
        LOG_INFO("User has selected PREF (Yellow) color.");
    }
    else if ( matchColor == eColor_SYM )
    {
        ROBOT.nav.setColor (eColor_SYM);
        ROBOT.setRGBled(BLUE, ON);
        LOG_INFO("User has selected SYM (Blue) color.");
    }
    else
    {
        ASSERT_TEXT(false, "StrategyThread::configureMatch : color should not be unknown.");
    }

    //Check selected strategy
    strategyId = strategyId_;
    ASSERT_TEXT(strategies[strategyId].functor != 0, "Selected strategy functor is null.");
    LOG_INFO("User has selected strategy " + strategies[strategyId].name + ".");
}

