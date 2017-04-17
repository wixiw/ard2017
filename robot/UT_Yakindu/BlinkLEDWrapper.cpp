/*
 * BlinkLEDWrapper.cpp
 *
 *  Created on: 17 avr. 2017
 *      Author: wix
 */

#include "BlinkLEDWrapper.h"
#include "BSP.h"

using namespace ard;

void BlinkLEDWrapper::setLed(sc_integer on)
{
    digitalWrite(LED_DUE_RX, on);
}

