/*
 * StrategyThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "StrategyThread.h"
#include "Robot2017.h"

#ifdef BUILD_STRATEGY

using namespace ard;
extern String getExeVersion();

StrategyThread::StrategyThread():
        Thread("Strategy", PRIO_STRATEGY, STACK_STRATEGY),
        strategyId(0),
        nbRegisteredStrats(0),
        robot(NULL)
{
    INIT_TABLE_TO_ZERO(strategies);
    stratInfo = apb_StratInfo2017_init_default;
    stratInfo.matchColor                = eColor_UNKNOWN;
    stratInfo.dispenserMonocolorNb      = 4;
    stratInfo.dispenserBicolorNb        = 4;
    stratInfo.dispenserOppNb            = 4;
    stratInfo.cylinderStart             = true;
    stratInfo.cylinderContainer         = true;
    stratInfo.cylinderCenter            = true;
    stratInfo.cylinderCorner            = true;
    stratInfo.cylinderCrater            = true;
    stratInfo.cylinderOppStart          = true;
    stratInfo.cylinderOppContainer      = true;
    stratInfo.cylinderOppCenter         = true;
    stratInfo.robotCylinderStockNb      = 0;
}

void StrategyThread::attachRobot(Robot2017* newRobot)
{
    ASSERT_TEXT(newRobot,"You should not attach a NULL robot");
    ASSERT_TEXT(robot == NULL, "You should not attach robot twice");
    robot=newRobot;
}

void StrategyThread::init()
{
    Thread::init();

    //init the event mailbox
    EventListener::init<1>();
}

void StrategyThread::displayIntroduction()
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

void StrategyThread::run()
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
        robot->buzzer().bip(1);
        robot->chrono.startMatch();
        strategies[strategyId].functor(Robot2017::getInstance());
    }
}

void StrategyThread::registerStrategy(String name, StrategyFunctor functor)
{
    ASSERT_TEXT(nbRegisteredStrats < NB_MAX_STRATEGIES, "Too many strategies registered.");
    strategies[nbRegisteredStrats].name = name;
    strategies[nbRegisteredStrats].functor = functor;
    nbRegisteredStrats++;
}

void StrategyThread::readUserInputs()
{   
    //Read color input
    eColor selectedColor = eColor_UNKNOWN;
    if ( robot->isPreferedColor() )
        selectedColor = eColor_PREF;
    else
        selectedColor = eColor_SYM;

    //Read strat config
    configureMatch(robot->getStrategyId(), selectedColor);
}

void StrategyThread::configureMatch(uint8_t strategyId_, eColor matchColor)
{
    robot->nav.setColor (matchColor);
    
    //Configure color
    if ( matchColor == eColor_PREF )
    {
        robot->setRGBled(YELLOW, ON);
        stratInfo.matchColor = eColor_PREF;
        LOG_INFO("User has selected PREF (Yellow) color.");
    }
    else if ( matchColor == eColor_SYM )
    {
        robot->nav.setColor (eColor_SYM);
        stratInfo.matchColor = eColor_SYM;
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

apb_StratInfo2017 const& StrategyThread::getStratInfo()
{
    return stratInfo;
}

void StrategyThread::withdraw(uint8_t nb, uint32_t& dispenserCount, const String& caller)
{
    uint32_t nbTransfered = 0;
    bool res = true;

    //nominal case
    if( nb <= dispenserCount )
    {
        nbTransfered = nb;
        dispenserCount -= nb;
    }
    //dispenser doesn't have enougth objects
    else
    {
        nbTransfered = dispenserCount;
        dispenserCount = 0;
        res = false;
    }

    stratInfo.robotCylinderStockNb += nbTransfered;

    //Too many item in robot
    if( 6 < stratInfo.robotCylinderStockNb )
    {
        stratInfo.robotCylinderStockNb = 6;
        res = false;
    }

    String log = caller + "(" + nb + "): taken=" + nbTransfered + ", remaining : "
            + dispenserCount + "/4, robot stock : "
            + stratInfo.robotCylinderStockNb + "/6.";

    if(res)
    {
        LOG_INFO(log);
    }
    else
    {
        LOG_ERROR(log);
    }
}

void StrategyThread::poo(uint8_t nb, uint8_t max, uint32_t& containerCount, const String& caller)
{
    uint32_t nbTransfered = nb;
    bool res = true;

    //Not enougth items in robot
    if( stratInfo.robotCylinderStockNb < nb )
    {
        nbTransfered = stratInfo.robotCylinderStockNb;
        res = false;
    }

    //Too many items for container
    if( max < containerCount + nbTransfered )
    {
        nbTransfered = max - containerCount;
        res = false;
    }

    containerCount += nbTransfered;
    stratInfo.robotCylinderStockNb -= nbTransfered;

    String log =caller + "(" + nb + "): pooed=" + nbTransfered + ", contained : " + containerCount + "/" + max
            + ", robot stock : " + stratInfo.robotCylinderStockNb + "/6.";

    if( res )
    {
        LOG_INFO(log);
    }
    else
    {
        LOG_ERROR(log);
    }
}

void StrategyThread::take(bool& objectPresent, const String& caller)
{
    uint32_t nbTransfered = 1;
    bool res = true;

    if( !objectPresent )
    {
        nbTransfered = 0;
        res = false;
    }

    objectPresent = false;
    stratInfo.robotCylinderStockNb += nbTransfered;

    //Too many item in robot
    if( 6 < stratInfo.robotCylinderStockNb )
    {
        stratInfo.robotCylinderStockNb = 6;
        res = false;
    }

    String log = caller + ": taken=" + nbTransfered + ", robot stock : "
            + stratInfo.robotCylinderStockNb + "/6.";

    if(res)
    {
        LOG_INFO(log);
    }
    else
    {
        LOG_ERROR(log);
    }
}

void StrategyThread::push(bool& objectPresent, const String& caller)
{
    objectPresent = false;
    LOG_INFO(caller);
}

void StrategyThread::informWithdraw_MonocolorDispenser(uint8_t nb)
{
    if(nb == 0)
    {
        nb = stratInfo.dispenserMonocolorNb;
    }
    withdraw(nb, stratInfo.dispenserMonocolorNb, "informWithdraw_MonocolorDispenser");
}

void StrategyThread::informWithdraw_BicolorDispenser(uint8_t nb)
{
    if(nb == 0)
    {
        nb = stratInfo.dispenserBicolorNb;
    }
    withdraw(nb, stratInfo.dispenserBicolorNb, "informWithdraw_BicolorDispenser");
}

void StrategyThread::informWithdraw_OppDispenser(uint8_t nb)
{
    if(nb == 0)
    {
        nb = stratInfo.dispenserOppNb;
    }
    withdraw(nb, stratInfo.dispenserOppNb, "informWithdraw_OppDispenser");
}

void StrategyThread::informPooed_MiddleCenter(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - stratInfo.containerMidleCenterNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 6, stratInfo.containerMidleCenterNb, "informPooed_MiddleCenter");
}

void StrategyThread::informPooed_MiddleOwn(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - stratInfo.containerMidleOwnNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 6, stratInfo.containerMidleOwnNb, "informPooed_MiddleOwn");
}

void StrategyThread::informPooed_MiddleOpp(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - stratInfo.containerMidleOppNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 6, stratInfo.containerMidleOppNb, "informPooed_MiddleOpp");
}

void StrategyThread::informPooed_Border(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(4 - stratInfo.containerBorderNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 4, stratInfo.containerBorderNb, "informPooed_Border");
}

void StrategyThread::informPooed_BorderOpp(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(4 - stratInfo.containerBorderOppNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 4, stratInfo.containerBorderOppNb, "informPooed_BorderOpp");
}

void StrategyThread::informPooed_Start(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(8 - stratInfo.containerStartNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 8, stratInfo.containerStartNb, "informPooed_Start");
}

void StrategyThread::informPooed_OnTable(uint8_t nb)
{
    if(nb == 0)
    {
        nb = stratInfo.robotCylinderStockNb;
    }
    uint32_t fakeCount = 0;
    poo(nb, 100, fakeCount, "informPooed_OnTable");
}

void StrategyThread::informTaken_Start()
{
    take( stratInfo.cylinderStart, "informTaken_Start");
}

void StrategyThread::informTaken_Container()
{
    take( stratInfo.cylinderContainer, "informTaken_Container");
}

void StrategyThread::informTaken_Center()
{
    take( stratInfo.cylinderCenter, "informTaken_Center");
}

void StrategyThread::informTaken_Corner()
{
    take( stratInfo.cylinderCorner, "informTaken_Corner");
}

void StrategyThread::informTaken_Crater()
{
    take( stratInfo.cylinderCrater, "informTaken_Crater");
}

void StrategyThread::informTaken_OppStart()
{
    take( stratInfo.cylinderOppStart, "informTaken_OppStart");
}

void StrategyThread::informTaken_OppContainer()
{
    take( stratInfo.cylinderOppContainer, "informTaken_OppContainer");
}

void StrategyThread::informTaken_OppCenter()
{
    take( stratInfo.cylinderOppCenter, "informTaken_OppCenter");
}

void StrategyThread::informPushedAway_Start()
{
    push( stratInfo.cylinderStart, "informPushedAway_Start");
}

void StrategyThread::informPushedAway_Container()
{
    push( stratInfo.cylinderContainer, "informPushedAway_Container");
}

void StrategyThread::informPushedAway_Center()
{
    push( stratInfo.cylinderCenter, "informPushedAway_Center");
}

void StrategyThread::informPushedAway_Corner()
{
    push( stratInfo.cylinderCorner, "informPushedAway_Corner");
}

void StrategyThread::informPushedAway_Crater()
{
    push( stratInfo.cylinderCrater, "informPushedAway_Crater");
}

void StrategyThread::informPushedAway_OppStart()
{
    push( stratInfo.cylinderOppStart, "informPushedAway_OppStart");
}

void StrategyThread::informPushedAway_OppContainer()
{
    push( stratInfo.cylinderOppContainer, "informPushedAway_OppContainer");
}

void StrategyThread::informPushedAway_OppCenter()
{
    push( stratInfo.cylinderOppCenter, "informPushedAway_OppCenter");
}
#endif
