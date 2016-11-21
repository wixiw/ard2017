/*
 * StrategyThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "StrategyThread.h"
#include "LogThread.h"
#include "Robot2017.h"

using namespace ard;

void
StrategyThread::init ()
{
  //create the thread
  g_ArdOs.createThread_Cpp("Strategy", *this, STACK_STRATEGY, PRIO_STRATEGY);
}

void
StrategyThread::run ()
{

//  Strategy_LedTest();
//  Strategy_ButtonTest ();
//  Strategy_OmronTest();

  LOG(INFO, "STRAT : Fake strategy.");
  ROBOT.nav.setColor (eColor::PREF);
  ROBOT.nav.setPosition(0,0,0);
  ROBOT.nav.goTo(300, 0, SENS_AV);
  LOG(INFO,"STRAT : Move order sent, waiting...");
  ROBOT.nav.wait();
  LOG(INFO,"STRAT : Return to start pos, waiting...");
  ROBOT.nav.goTo(0, 0, SENS_AV);
  ROBOT.nav.wait();
  LOG(INFO,"STRAT : Finished.");
}

void
StrategyThread::Strategy_LedTest ()
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
StrategyThread::Strategy_ButtonTest ()
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
StrategyThread::Strategy_OmronTest ()
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


