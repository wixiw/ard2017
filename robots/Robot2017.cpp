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
    log(LogThread::getInstance()),
    hmi (50 /* ms */),
    stepperG(AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR),
    stepperD(AccelStepper::DRIVER, PAPD_STEP, PAPD_DIR)
{
  // TODO Auto-generated constructor stub
}

void
Robot2017::boot ()
{
  //Init debug serial link
  Serial.begin (/*baurate = */250000);

  //Threads init
  log.init ();
  teleop.init ();
  strategy.init();

  //Peripheral initialization
  hmi.init();
  hmi.ledDue_Tx.slowBlink();
  pinMode(PAP_ENABLE, OUTPUT);

  g_ArdOs.init();
  g_ArdOs.start(); //this function never ends
}
