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

StrategyThread::StrategyThread (Robot2017& robot) :
    r (robot)
{
}

void
StrategyThread::init ()
{
  //create the thread
  g_ArdOs.createThread_Cpp("Strategy", *this, STACK_STRATEGY, PRIO_STRATEGY);
}

void
StrategyThread::run ()
{
  LOG(INFO, "STRAT : Fake strategy.");
  r.nav.setColor (eColor::PREF);

//  Strategy_LedTest();
//  Strategy_ButtonTest ();
  Strategy_OmronTest();

//  r.nav.setPosition(0,0,0);
//  r.nav.goTo(20,00, SENS_AV);
//
//  LOG(INFO,"STRAT : Move order sent, waiting...");
//  r.nav.wait();
//  LOG(INFO,"STRAT : Finished.");
}

void
StrategyThread::Strategy_ButtonTest ()
{
  while (1)
    {
      if (r.hmi.matchColor.readRaw ())
	{
	  r.hmi.led1.on ();
	}
      else
	{
	  r.hmi.led1.off ();
	}

      if (r.hmi.user1.readRaw ())
	{
	  r.hmi.led2.on ();
	}
      else
	{
	  r.hmi.led2.off ();
	}

      if (r.hmi.user2.readRaw ())
	{
	  r.hmi.led3.on ();
	}
      else
	{
	  r.hmi.led3.off ();
	}

      if (r.hmi.start.readRaw ())
	{
	  r.hmi.led4.on ();
	}
      else
	{
	  r.hmi.led4.off ();
	}

      delay (50);
    }
}

void
StrategyThread::Strategy_OmronTest ()
{
  while (1)
    {
      if (r.nav.getOmronState_FL ())
	r.hmi.led1.on ();
      else
	r.hmi.led1.off ();

      if (r.nav.getOmronState_FR ())
	r.hmi.led2.on ();
      else
	r.hmi.led2.off ();

      if (r.nav.getOmronState_RL ())
	r.hmi.led3.on ();
      else
	r.hmi.led3.off ();

      if (r.nav.getOmronState_RR ())
	r.hmi.led4.on ();
      else
	r.hmi.led4.off ();

      delay (50);
    }
}

void
StrategyThread::Strategy_LedTest ()
{
  int WAIT = 200;

  while (1)
    {
      r.hmi.ledRGB.set (RED, ON);
      r.hmi.led4.off ();
      r.hmi.led1.on ();
      delay (WAIT);
      r.hmi.ledRGB.set (GREEN, ON);
      r.hmi.led1.off ();
      r.hmi.led2.on ();
      delay (WAIT);
      r.hmi.ledRGB.set (BLUE, ON);
      r.hmi.led2.off ();
      r.hmi.led3.on ();
      delay (WAIT);
      r.hmi.ledRGB.set (WHITE, ON);
      r.hmi.led3.off ();
      r.hmi.led4.on ();
      delay (WAIT);
    }
}

