/*
 * ActuatorThread.cpp
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#include "ActuatorThread.h"
#include "LogThread.h"

using namespace ard;

ActuatorThread::ActuatorThread ()
{
}

void
ActuatorThread::init ()
{
  //create the thread
  g_ArdOs.createPeriodicThread_Cpp("Actuators", *this, STACK_ACTUATORS, PRIO_ACTUATORS, PERIOD_ACTUATORS);
}

void
ActuatorThread::run ()
{
  //Nav update
  nav.update();
  //Actuators update
  //Strat signals update
}
