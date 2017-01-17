/* 
* TestStrategies.cpp
*
* Created: 08/12/2016 00:17:42
* Author: wix
*/

#include "TestStrategies.h"
#include "../Robot2017.h"

using namespace ard;

void
ard::Strategy_LedTest ()
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


void
ard::Strategy_ButtonTest ()
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

void
ard::Strategy_OmronTest ()
{
	LOG_INFO("STRAT : Strategy_OmronTest.");

	while (1)
	{
		if (ROBOT.nav.getOmronState_FL ())
		ROBOT.setLed(LED1, eLedState::ON);
		else
		ROBOT.setLed(LED1, eLedState::OFF);

		if (ROBOT.nav.getOmronState_FR ())
		ROBOT.setLed(LED2, eLedState::ON);
		else
		ROBOT.setLed(LED2, eLedState::OFF);

		if (ROBOT.nav.getOmronState_RL ())
		ROBOT.setLed(LED3, eLedState::ON);
		else
		ROBOT.setLed(LED3, eLedState::OFF);

		if (ROBOT.nav.getOmronState_RR ())
		ROBOT.setLed(LED4, eLedState::ON);
		else
		ROBOT.setLed(LED4, eLedState::OFF);

		ArdOs::sleepMs (50);
	}
}
