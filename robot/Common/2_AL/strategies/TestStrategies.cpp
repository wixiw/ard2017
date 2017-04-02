/* 
 * TestStrategies.cpp
 *
 * Created: 08/12/2016 00:17:42
 * Author: wix
 */

#include "Strategies.h"
#include "../Robot2017.h"

#ifdef BUILD_STRATEGY

using namespace ard;

void ard::Strategy_LedTest(Robot2017& robot)
{
    auto WAIT = 200;
    LOG_INFO("STRAT : Strategy_LedTest.");

    while (1)
    {
        robot.setRGBled(RED, ON);
        robot.setLed(LED4, eLedState::OFF);
        robot.setLed(LED1, eLedState::ON);
        ArdOs::sleepMs (WAIT);
        robot.setRGBled(GREEN, ON);
        robot.setLed(LED1, eLedState::OFF);
        robot.setLed(LED2, eLedState::ON);
        ArdOs::sleepMs (WAIT);
        robot.setRGBled(BLUE, ON);
        robot.setLed(LED2, eLedState::OFF);
        robot.setLed(LED3, eLedState::ON);
        ArdOs::sleepMs (WAIT);
        robot.setRGBled(WHITE, ON);
        robot.setLed(LED3, eLedState::OFF);
        robot.setLed(LED4, eLedState::ON);
        ArdOs::sleepMs (WAIT);
    }
}

void ard::Strategy_ButtonTest(Robot2017& robot)
{
    LOG_INFO("STRAT : Strategy_ButtonTest.");
    while (1)
    {
        if (robot.isPreferedColor())
        {
            robot.setLed(LED1, eLedState::ON);
            robot.actuators.claws.arm();
        }
        else
        {
            robot.setLed(LED1, eLedState::OFF);
            robot.actuators.claws.release();
        }

        if (robot.getStrategyId() & 0x02)
        {
            robot.setLed(LED2, eLedState::ON);
            robot.actuators.claws.open();
        }
        else
        {
            robot.setLed(LED2, eLedState::OFF);
            robot.actuators.claws.close();
        }

        if (robot.getStrategyId() & 0x02)
        {
            robot.setLed(LED3, eLedState::ON);
        }
        else
        {
            robot.setLed(LED3, eLedState::OFF);
        }

        if (robot.isStartPlugged())
        {
            robot.setLed(LED4, eLedState::ON);
        }
        else
        {
            robot.setLed(LED4, eLedState::OFF);
        }

        ArdOs::sleepMs (50);
    }
}

void ard::Strategy_OmronTest(Robot2017& robot)
{
    LOG_INFO("STRAT : Strategy_OmronTest.");

    apb_NavState nav;

    while (1)
    {
        nav = robot.nav.getState();

        if (nav.omronFL)
        robot.setLed(LED1, eLedState::ON);
        else
        robot.setLed(LED1, eLedState::OFF);

        if (nav.omronFR)
        robot.setLed(LED2, eLedState::ON);
        else
        robot.setLed(LED2, eLedState::OFF);

        if (nav.omronRL)
        robot.setLed(LED3, eLedState::ON);
        else
        robot.setLed(LED3, eLedState::OFF);

        if (nav.omronRR)
        robot.setLed(LED4, eLedState::ON);
        else
        robot.setLed(LED4, eLedState::OFF);

        ArdOs::sleepMs (50);
    }
}

void ard::Strategy_CalibTest(Robot2017& robot)
{
    auto evt_startOut = robot.getStartOutEvt();
    auto evt_teleopStart = robot.getRemoteControlEvt(EVT_START_MATCH);
    auto eventMotor = EventListener();
    eventMotor.init<1>();

    LOG_INFO("STRAT : Strategy_Alpha.");

    //robot.nav.setPosition(610,820,-90);

    while(1)
    {
        LOG_INFO("STRAT : Move order sent");
        robot.nav.setPosition(0,0,0);
        robot.nav.goTo(0, 300, eDir_FORWARD);
        robot.nav.wait();

        //wait for start withdraw or a teleop command to start the match
        {
            LOG_INFO("Waiting start to redo...");
            IEvent* evts[] =
            {   evt_startOut, evt_teleopStart};
            eventMotor.waitEvents(evts, 2); //returned event is not read as we don't care, result will be the same
        }

    }

    LOG_INFO("STRAT : Finished.");

    robot.dieMotherFucker();
}

void ard::Strategy_MotionTest(Robot2017& robot)
{
    LOG_INFO("STRAT : Strategy_Alpha.");

    //robot.nav.setPosition(610,820,-90);

    while (1)
    {
        LOG_INFO("STRAT : Move order 1 sent");
        robot.nav.goToCap(0, 0, 0, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 2 sent");
        robot.nav.goToCap(0, 0, 90, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 3 sent");
        robot.nav.goToCap(300, 0, 90, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 4 sent");
        robot.nav.goTo(0, 0, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 5 sent");
        robot.nav.goTo(0, 300, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 6 sent");
        robot.nav.goTo(300, 300, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 7 sent");
        robot.nav.goTo(300, 0, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 8 sent");
        robot.nav.goTo(0, 0, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 9 sent");
        robot.nav.goTo(300, 300, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 10 sent");
        robot.nav.goTo(0, 0, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 11 sent");
        robot.nav.goToCap(300, 300, 0, eDir_BACKWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 12 sent");
        robot.nav.goTo(0, 0, eDir_BACKWARD);
        robot.nav.wait();

        ArdOs::sleepMs(2000);
    }

    LOG_INFO("STRAT : Finished.");

    robot.dieMotherFucker();
}

#endif
