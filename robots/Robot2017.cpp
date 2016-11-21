/*
 * Robot2017.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "Robot2017.h"
#include "BSP.h"

using namespace ard;

//singleton instanciation
Robot2017 Robot2017::instance = Robot2017 ();

//Use this interrupt to execute periodic stuff that shall run at a very high frequency (typically steppers)
//Obviously at such a frequence a Thread is too heavy as the context-switch duration would be higher than the period
void veryFast_interrupt()
{
//  digitalWrite(DEBUG_1, 1); //uncomment to check period and delay with oscilloscope
  Robot2017::getInstance().nav.updateFromInterrupt();
//  digitalWrite(DEBUG_1, 0); //uncomment to check period and delay with oscilloscope
}

//Use this interrupt to execute period stuff that shall run at a high frequency
//At this frequence a Thread is quite heavy as the context-switch duration would be roughtly equal to the period
void fast_interrupt()
{
//  digitalWrite(DEBUG_2, 1); //uncomment to check period and delay with oscilloscope
  gpioToolsIsrCallback(PERIOD_FAST_IT_US);
//  digitalWrite(DEBUG_2, 0); //uncomment to check period and delay with oscilloscope
}

Robot2017::Robot2017 () :
    hmi (50 /* ms */),
    log(LogThread::getInstance()),
    teleop(),
    actuators(),
    strategy(),
    nav()
{
  actuators.addMiniThread(&nav);
}

void
Robot2017::boot ()
{
  init_bsp();

  //Map fast periodic functions to timers interrupts
  Timer6.attachInterrupt(veryFast_interrupt);
  Timer7.attachInterrupt(fast_interrupt);

  //Init debug serial link
  Serial.begin (/*baurate = */250000);

  //Threads init
  hmi.init();
  hmi.ledDue_Tx.slowBlink(); //TODO pour le debug pour verifier que le thread est vivant

  log.init ();
  teleop.init ();
  actuators.init ();
  strategy.init();

  //Configure and start periodic timers
  Timer6.start(PERIOD_VERY_FAST_IT_US);
  Timer7.start(PERIOD_FAST_IT_US);

  g_ArdOs.init();
  g_ArdOs.start(); //this function never ends
}
