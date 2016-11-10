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

StrategyThread::StrategyThread (Robot2017& robot):
     r(robot)
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
  LOG(INFO,"STRAT : Fake strategy.");
  r.nav.setColor(COLOR_PREF);
  r.nav.setPosition(0,0,0);
  r.nav.goTo(20,00, SENS_AV);

  LOG(INFO,"STRAT :Move order sent, waiting...");
  r.nav.wait();
  LOG(INFO,"STRAT :Finished.");
}
