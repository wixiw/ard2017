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
// Uncomment for oscilloscope debug and tests
//    static int8_t toggle = 0;
//    toggle = 1 - toggle;
//    digitalWrite(DEBUG_2, toggle);

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
