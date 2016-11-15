/*
 * ActuatorThread.cpp
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#include "ActuatorThread.h"
#include "LogThread.h"
#include "Robot2017.h"

using namespace ard;

ActuatorThread::ActuatorThread (Robot2017& robot):
    nextRank(0),
    r(robot)
{
}

void
ActuatorThread::init ()
{
  //create the thread
  g_ArdOs.createPeriodicThread_Cpp("Actuators", *this, STACK_ACTUATORS, PRIO_ACTUATORS, PERIOD_ACTUATORS);

  for(int i = 0 ; i < nextRank ; ++i)
    {
      threadlets[i]->init();
    }
}

void
ActuatorThread::run ()
{
  for(int i = 0 ; i < nextRank ; ++i)
    {
      threadlets[i]->update(PERIOD_ACTUATORS);
    }
}

void
ActuatorThread::addMiniThread(IMiniPeriodicThread* threadlet )
{
  ardAssert(nextRank < NB_MAX_MINI_THREADS, "Actuators : too many mini threads");
  threadlets[nextRank] = threadlet;
  nextRank++;
}
