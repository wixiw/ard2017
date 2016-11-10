/*
 * Robot2017.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "Robot2017.h"
#include "K_pinout.h"

using namespace ard;

Robot2017::Robot2017 () :
    hmi (50 /* ms */),
    log(LogThread::getInstance()),
    teleop(),
    actuators(),
    strategy(*this)
{
  actuators.addMiniThread(&nav);
}

void
Robot2017::boot ()
{
  //Init debug serial link
  Serial.begin (/*baurate = */250000);

  //Threads init
  hmi.init();
  hmi.ledDue_Tx.slowBlink(); //TODO pour le debug pour verifier que le thread est vivant

  log.init ();
  teleop.init ();
  actuators.init ();
  strategy.init();

  g_ArdOs.init();
  g_ArdOs.start(); //this function never ends
}
