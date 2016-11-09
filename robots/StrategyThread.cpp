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
  g_ArdOs.createThread_Cpp("Strategy", *this, STACK_STRATEGY, PRIO_STRATEGY);
}

void
StrategyThread::run ()
{
  LOG(INFO,"Fake strategy running.");
}
