/*
 * TeleopThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "TeleopThread.h"

namespace ard
{

  TeleopThread::TeleopThread ()
  {
  }

  void
  TeleopThread::init ()
  {
    //create the thread
    g_ArdOs.createThread_Cpp("Teleop", *this, STACK_LOG, PRIO_LOG);
  }

  void
  TeleopThread::run ()
  {
    while (1)
      {
	while (Serial.available ())
	  {
	    Serial.read ();
	    //TODO do something
	    g_ArdOs.displayStats();
	}

      vTaskDelay(1);
    }
}

}
/* namespace ard */
