/*
 * StrategyThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "StrategyThread.h"
#include "LogThread.h"

using namespace ard;

StrategyThread::StrategyThread ()
{
}

void
StrategyThread::init ()
{
  //create the thread
  g_ArdOs.createPeriodicThread_Cpp("Teleop", *this, STACK_STRATEGY, PRIO_STRATEGY, 1000 /* period in ms*/);
}

void
StrategyThread::run ()
{
  LOG(INFO,"Fake strategy running.");
}
