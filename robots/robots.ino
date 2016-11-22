/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "Robot2017.h"

using namespace ard;

#include <FreeRTOS_ARM.h>
TaskHandle_t xIdleHandle = NULL;

//------------------------------------------------------------------------------
void
setup ()
{
  ROBOT.boot();
}

//------------------------------------------------------------------------------
void
loop ()
{
  // idle loop has a very small, configMINIMAL_STACK_SIZE, stack
  // loop must never block
  while(1);
}
