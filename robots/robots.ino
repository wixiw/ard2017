/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "RSP.h"

using namespace ard;

void
Task1 ()
{
  while (1)
    {
      while (Serial.available())
	{
	  Serial.read();
	  g_Log.log(INFO, "8=>");
	}

      vTaskDelay(1);
    }
}

//------------------------------------------------------------------------------
void
setup ()
{
  g_ArdOs.init();
  g_ArdOs.createThread_C("Task1", Task1, 200, 2);
  g_Log.init();
  g_ArdOs.start();
}

//------------------------------------------------------------------------------
void
loop ()
{
  // idle loop has a very small, configMINIMAL_STACK_SIZE, stack
  // loop must never block
  // the heartbeat movement is a proof that the CPU is not overused as the minimal priority task is executed
  g_ArdOs.kickHeartbeat();
}
