/* 
 * TestStrategies.cpp
 *
 * Created: 08/12/2016 00:17:42
 * Author: wix
 */

#include "Strategies.h"
#include "../Robot2017.h"

using namespace ard;

void ard::Strategy_LedTest()
{
    auto WAIT = 200;
    LOG_INFO("STRAT : Strategy_LedTest.");

    while (1)
    {
        ROBOT.setRGBled(RED, ON);
        ROBOT.setLed(LED4, eLedState::OFF);
        ROBOT.setLed(LED1, eLedState::ON);
        ArdOs::sleepMs (WAIT);
        ROBOT.setRGBled(GREEN, ON);
        ROBOT.setLed(LED1, eLedState::OFF);
        ROBOT.setLed(LED2, eLedState::ON);
        ArdOs::sleepMs (WAIT);
        ROBOT.setRGBled(BLUE, ON);
        ROBOT.setLed(LED2, eLedState::OFF);
        ROBOT.setLed(LED3, eLedState::ON);
        ArdOs::sleepMs (WAIT);
        ROBOT.setRGBled(WHITE, ON);
        ROBOT.setLed(LED3, eLedState::OFF);
        ROBOT.setLed(LED4, eLedState::ON);
        ArdOs::sleepMs (WAIT);
    }
}

void ard::Strategy_ButtonTest()
{
    LOG_INFO("STRAT : Strategy_ButtonTest.");
    while (1)
    {
        if (ROBOT.isPreferedColor())
        {
            ROBOT.setLed(LED1, eLedState::ON);
            ROBOT.actuators.claws.arm();
        }
        else
        {
            ROBOT.setLed(LED1, eLedState::OFF);
            ROBOT.actuators.claws.release();
        }

        if (ROBOT.getStrategyId() & 0x02)
        {
            ROBOT.setLed(LED2, eLedState::ON);
            ROBOT.actuators.claws.open();
        }
        else
        {
            ROBOT.setLed(LED2, eLedState::OFF);
            ROBOT.actuators.claws.close();
        }

        if (ROBOT.getStrategyId() & 0x02)
        {
            ROBOT.setLed(LED3, eLedState::ON);
        }
        else
        {
            ROBOT.setLed(LED3, eLedState::OFF);
        }

        if (ROBOT.isStartPlugged())
        {
            ROBOT.setLed(LED4, eLedState::ON);
        }
        else
        {
            ROBOT.setLed(LED4, eLedState::OFF);
        }

        ArdOs::sleepMs (50);
    }
}

void ard::Strategy_OmronTest()
{
    LOG_INFO("STRAT : Strategy_OmronTest.");

    apb_NavState nav;

    while (1)
    {
        nav = ROBOT.nav.getState();

        if (nav.omronFL)
        ROBOT.setLed(LED1, eLedState::ON);
        else
        ROBOT.setLed(LED1, eLedState::OFF);

        if (nav.omronFR)
        ROBOT.setLed(LED2, eLedState::ON);
        else
        ROBOT.setLed(LED2, eLedState::OFF);

        if (nav.omronRL)
        ROBOT.setLed(LED3, eLedState::ON);
        else
        ROBOT.setLed(LED3, eLedState::OFF);

        if (nav.omronRR)
        ROBOT.setLed(LED4, eLedState::ON);
        else
        ROBOT.setLed(LED4, eLedState::OFF);

        ArdOs::sleepMs (50);
    }
}

void ard::Strategy_CalibTest()
{
    auto evt_startOut = ROBOT.getStartOutEvt();
    auto evt_teleopStart = ROBOT.getRemoteControlEvt(EVT_START_MATCH);
    auto eventMotor = EventListener();
    eventMotor.init<1>();

    LOG_INFO("STRAT : Strategy_Alpha.");

    //ROBOT.nav.setPosition(610,820,-90);

    while(1)
    {
        LOG_INFO("STRAT : Move order sent");
        ROBOT.nav.setPosition(0,0,0);
        ROBOT.nav.goTo(0, 300, eDir_FORWARD);
        ROBOT.nav.wait();

        //wait for start withdraw or a teleop command to start the match
        {
            LOG_INFO("Waiting start to redo...");
            IEvent* evts[] =
            {   evt_startOut, evt_teleopStart};
            eventMotor.waitEvents(evts, 2); //returned event is not read as we don't care, result will be the same
        }

    }

    LOG_INFO("STRAT : Finished.");

    ROBOT.dieMotherFucker();
}

void ard::Strategy_MotionTest()
{
    LOG_INFO("STRAT : Strategy_Alpha.");

    //ROBOT.nav.setPosition(610,820,-90);

    while (1)
    {
        LOG_INFO("STRAT : Move order 1 sent");
        ROBOT.nav.goToCap(0, 0, 0, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 2 sent");
        ROBOT.nav.goToCap(0, 0, 90, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 3 sent");
        ROBOT.nav.goToCap(300, 0, 90, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 4 sent");
        ROBOT.nav.goTo(0, 0, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 5 sent");
        ROBOT.nav.goTo(0, 300, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 6 sent");
        ROBOT.nav.goTo(300, 300, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 7 sent");
        ROBOT.nav.goTo(300, 0, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 8 sent");
        ROBOT.nav.goTo(0, 0, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 9 sent");
        ROBOT.nav.goTo(300, 300, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 10 sent");
        ROBOT.nav.goTo(0, 0, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 11 sent");
        ROBOT.nav.goToCap(300, 300, 0, eDir_BACKWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 12 sent");
        ROBOT.nav.goTo(0, 0, eDir_BACKWARD);
        ROBOT.nav.wait();

        ArdOs::sleepMs(2000);
    }

    LOG_INFO("STRAT : Finished.");

    ROBOT.dieMotherFucker();
}
