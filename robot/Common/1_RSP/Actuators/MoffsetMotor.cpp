/*
 * MoffsetMotor.cpp
 *
 *  Created on: 24 mai 2017
 *      Author: lambert.w
 */

#include "MoffsetMotor.h"
#include "BSP.hpp"
using namespace ard;

MoffsetMotor::MoffsetMotor(uint8_t pin):
		pin(pin)
{
}

void MoffsetMotor::turn(bool on)
{
	digitalWrite(pin, on);
}


