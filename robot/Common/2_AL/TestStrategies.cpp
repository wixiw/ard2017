/* 
* TestStrategies.cpp
*
* Created: 08/12/2016 00:17:42
* Author: wix
*/

#include "TestStrategies.h"
#include "Robot2017.h"

using namespace ard;

void
ard::Strategy_LedTest ()
{
	int WAIT = 200;
	LOG(INFO, "STRAT : Strategy_LedTest.");

	while (1)
	{
		ROBOT.hmi.ledRGB.set (RED, ON);
		ROBOT.hmi.led4.off ();
		ROBOT.hmi.led1.on ();
		delay (WAIT);
		ROBOT.hmi.ledRGB.set (GREEN, ON);
		ROBOT.hmi.led1.off ();
		ROBOT.hmi.led2.on ();
		delay (WAIT);
		ROBOT.hmi.ledRGB.set (BLUE, ON);
		ROBOT.hmi.led2.off ();
		ROBOT.hmi.led3.on ();
		delay (WAIT);
		ROBOT.hmi.ledRGB.set (WHITE, ON);
		ROBOT.hmi.led3.off ();
		ROBOT.hmi.led4.on ();
		delay (WAIT);
	}
}


void
ard::Strategy_ButtonTest ()
{
	LOG(INFO, "STRAT : Strategy_ButtonTest.");
	while (1)
	{
		if (ROBOT.hmi.matchColor.read ())
		{
			ROBOT.hmi.led1.on ();
		}
		else
		{
			ROBOT.hmi.led1.off ();
		}

		if (ROBOT.hmi.user1.read ())
		{
			ROBOT.hmi.led2.on ();
		}
		else
		{
			ROBOT.hmi.led2.off ();
		}

		if (ROBOT.hmi.user2.read ())
		{
			ROBOT.hmi.led3.on ();
		}
		else
		{
			ROBOT.hmi.led3.off ();
		}

		if (ROBOT.hmi.start.read ())
		{
			ROBOT.hmi.led4.on ();
		}
		else
		{
			ROBOT.hmi.led4.off ();
		}

		delay (50);
	}
}

void
ard::Strategy_OmronTest ()
{
	LOG(INFO, "STRAT : Strategy_OmronTest.");

	while (1)
	{
		if (ROBOT.nav.getOmronState_FL ())
		ROBOT.hmi.led1.on ();
		else
		ROBOT.hmi.led1.off ();

		if (ROBOT.nav.getOmronState_FR ())
		ROBOT.hmi.led2.on ();
		else
		ROBOT.hmi.led2.off ();

		if (ROBOT.nav.getOmronState_RL ())
		ROBOT.hmi.led3.on ();
		else
		ROBOT.hmi.led3.off ();

		if (ROBOT.nav.getOmronState_RR ())
		ROBOT.hmi.led4.on ();
		else
		ROBOT.hmi.led4.off ();

		delay (50);
	}
}