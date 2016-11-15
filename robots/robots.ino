/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "RSP.h"
#include "Robot2017.h"

using namespace ard;

Robot2017 robot;

//Use this interrupt to execute periodic stuff that shall run at a very high frequency (typically steppers)
//Obviously at such a frequence a Thread is too heavy as the context-switch duration would be higher than the period
void veryFast_interrupt()
{
  robot.nav.updateFromInterrupt();
}

//Use this interrupt to execute period stuff that shall run at a high frequency
//At this frequence a Thread is quite heavy as the context-switch duration would be roughtly equal to the period
void fast_interrupt()
{
  gpioToolsIsrCallback(PERIOD_FAST_IT_US);
}

//------------------------------------------------------------------------------
void
setup ()
{
  robot.boot();

  //stepper update IT mapping
  Timer6.attachInterrupt(veryFast_interrupt).setFrequency(PERIOD_VERY_FAST_IT_US).start();
  Timer7.attachInterrupt(fast_interrupt).setFrequency(PERIOD_FAST_IT_US).start();
}

//------------------------------------------------------------------------------
void
loop ()
{
  //as looop is called in loop, protect the "first run section" to be executed once
  static bool once = false;
  if( !once )
    {
      //robot.log.log(INFO, "Robot boot completed sucessfully.");
      once = true;
    }

  // idle loop has a very small, configMINIMAL_STACK_SIZE, stack
  // loop must never block
  // the heartbeat movement is a proof that the CPU is not overused as the minimal priority task is executed
  g_ArdOs.kickHeartbeat();
}
