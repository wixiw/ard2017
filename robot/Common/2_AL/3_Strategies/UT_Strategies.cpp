/* 
 * TestStrategies.cpp
 *
 * Created: 08/12/2016 00:17:42
 * Author: wix
 */

#include "LinearStrategies.h"
#include "Robot.h"

using namespace ard;

void ard::Strategy_LedTest(Robot2017& robot)
{
    auto WAIT = 200;
    LOG_INFO("STRAT : Strategy_LedTest.");

    while (1)
    {
        robot.hmi.setRGBled(RED, ON);
        robot.hmi.setLed(LED4, eLedState::OFF);
        robot.hmi.setLed(LED1, eLedState::ON);
        ArdOs::sleepMs (WAIT);
        robot.hmi.setRGBled(GREEN, ON);
        robot.hmi.setLed(LED1, eLedState::OFF);
        robot.hmi.setLed(LED2, eLedState::ON);
        ArdOs::sleepMs (WAIT);
        robot.hmi.setRGBled(BLUE, ON);
        robot.hmi.setLed(LED2, eLedState::OFF);
        robot.hmi.setLed(LED3, eLedState::ON);
        ArdOs::sleepMs (WAIT);
        robot.hmi.setRGBled(WHITE, ON);
        robot.hmi.setLed(LED3, eLedState::OFF);
        robot.hmi.setLed(LED4, eLedState::ON);
        ArdOs::sleepMs (WAIT);
    }
}

void ard::Strategy_ButtonTest(Robot2017& robot)
{
    LOG_INFO("STRAT : Strategy_ButtonTest.");
    while (1)
    {
        if (robot.hmi.isColorSwitchOnPrefered())
        {
            robot.hmi.setLed(LED1, eLedState::ON);
        }
        else
        {
            robot.hmi.setLed(LED1, eLedState::OFF);
        }

        if (robot.hmi.getStrategyId() & 0x02)
        {
            robot.hmi.setLed(LED2, eLedState::ON);
        }
        else
        {
            robot.hmi.setLed(LED2, eLedState::OFF);
        }

        if (robot.hmi.getStrategyId() & 0x02)
        {
            robot.hmi.setLed(LED3, eLedState::ON);
        }
        else
        {
            robot.hmi.setLed(LED3, eLedState::OFF);
        }

        if (robot.hmi.isStartPlugged())
        {
            robot.hmi.setLed(LED4, eLedState::ON);
        }
        else
        {
            robot.hmi.setLed(LED4, eLedState::OFF);
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
        nav = robot.nav.serealize();

        if (nav.omronFL)
        robot.hmi.setLed(LED1, eLedState::ON);
        else
        robot.hmi.setLed(LED1, eLedState::OFF);

        if (nav.omronFR)
        robot.hmi.setLed(LED2, eLedState::ON);
        else
        robot.hmi.setLed(LED2, eLedState::OFF);

        if (nav.omronRL)
        robot.hmi.setLed(LED3, eLedState::ON);
        else
        robot.hmi.setLed(LED3, eLedState::OFF);

        if (nav.omronRR)
        robot.hmi.setLed(LED4, eLedState::ON);
        else
        robot.hmi.setLed(LED4, eLedState::OFF);

        ArdOs::sleepMs (50);
    }
}

void ard::Strategy_CalibRot(Robot2017& robot)
{
    LOG_ERROR("Strategy_CalibRot is not implemented.");
}

void ard::Strategy_CalibLin(Robot2017& robot)
{
    LOG_INFO("STRAT : Strategy_CalibRot.");

    while( !robot.hmi.isStartPlugged() )
    {
        ArdOs::sleepMs (50);
    }

    while( robot.hmi.isStartPlugged() )
    {
        ArdOs::sleepMs (50);
    }

    //robot.nav.setPosition(610,820,-90);

    while(1)
    {
        LOG_INFO("STRAT : Move order sent");
        robot.nav.setPosition(0,0,0);
        robot.nav.goTo(0, 300, eDir_FORWARD);
        robot.nav.wait();

        while( !robot.hmi.isStartPlugged() )
        {
            ArdOs::sleepMs (50);
        }

        while( robot.hmi.isStartPlugged() )
        {
            ArdOs::sleepMs (50);
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

