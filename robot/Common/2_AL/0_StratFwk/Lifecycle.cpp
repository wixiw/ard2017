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

Lifecycle::Lifecycle():
        Thread("Strategy", PRIO_STRATEGY, STACK_STRATEGY),
        strategyId(0),
        nbRegisteredStrats(0),
        robot(NULL)
{
    INIT_TABLE_TO_ZERO(strategies);
}

void Lifecycle::attachRobot(Robot2017* newRobot)
{
    ASSERT_TEXT(newRobot,"You should not attach a NULL robot");
    ASSERT_TEXT(robot == NULL, "You should not attach robot twice");
    robot=newRobot;
}

void Lifecycle::init()
{
    Thread::init();

    //init the event mailbox
    EventListener::init<1>();
}

void Lifecycle::displayIntroduction()
{
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

    LOG_INFO("Available strategies : ");
    for (int i = 0; i < NB_MAX_STRATEGIES; ++i)
    {
        if( strategies[i].functor != NULL )
        {
            sleepMs(10);//Let Log thread do its job
            LOG_INFO("    [" + String(i) + "]: " + strategies[i].name);
        }
    }

    robot->buzzer().bip(2);
    robot->buzzer().wait();
}

void Lifecycle::run()
{
    auto evt_startIn = robot->getStartInEvt();
    auto evt_startOut = robot->getStartOutEvt();
    auto evt_teleopConfigure = robot->getRemoteControlEvt(EVT_CONFIGURE);
    auto evt_teleopStart = robot->getRemoteControlEvt(EVT_START_MATCH);

    displayIntroduction();

    //wait for start insertion or teleop command
    {
        LOG_INFO("Waiting for user to choose strategy/color and to insert start...");
        IEvent* evts[] =
        {   evt_startIn, evt_teleopConfigure};
        auto triggeredEvent = waitEvents(evts, 2);
        robot->buzzer().bip(1);

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
        auto triggeredEvent = waitEvents(evts, 2); //returned event is not read as we don't care, result will be the same

        //Avoidance is activated after start so that it is deactivated in simulation
        if( triggeredEvent == evt_startOut)
        {
            robot->nav.enableAvoidance(true);
        }

        //Execute selected strategy
        robot->chrono.startMatch();
        robot->buzzer().bip(1);
        strategies[strategyId].functor(Robot2017::getInstance());
    }
}

void Lifecycle::registerStrategy(String name, StrategyFunctor functor)
{
    ASSERT_TEXT(nbRegisteredStrats < NB_MAX_STRATEGIES, "Too many strategies registered.");
    strategies[nbRegisteredStrats].name = name;
    strategies[nbRegisteredStrats].functor = functor;
    nbRegisteredStrats++;
}

void Lifecycle::readUserInputs()
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
    ASSERT_TEXT(strategies[strategyId].functor != 0, "Selected strategy functor is null.");
    strategyId = strategyId_;
    LOG_INFO(String("User has selected strategy [") + strategyId_ + "] " + strategies[strategyId].name + ".");
}


#endif
