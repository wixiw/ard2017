/*
 * HomingServo.cpp
 *
 *  Created on: 28 avr. 2017
 *      Author: wix
 */

#include "HomingServo.h"
#include "ArdOs.h"
#include "BSP.hpp"

using namespace ard;

HomingServo::HomingServo(String const& name, int servoOutPin, FilteredInput* pinMin, FilteredInput* pinMax, bool inverted):
        AccelServo(name, servoOutPin, 0, 1000),
        pinMin(pinMin),
        pinMax(pinMax),
        inverted(inverted),
        state(IDLE)
{
    //At least one pin shall be non null
    ASSERT(pinMin != NULL || pinMax != NULL);
}

void HomingServo::update(DelayMs sinceLastCall)
{
    switch(state)
    {
    case HOMING_MIN:
        if(pinMin->read() || AccelServo::isTargetReached())
            state = IDLE;
        break;
    case HOMING_MAX:
        if(pinMax->read())
            state = IDLE;
        if(AccelServo::isTargetReached())
            state = IDLE;
        break;
    case MOVING:
        if(AccelServo::isTargetReached())
            state = IDLE;
        break;
    default:
        break;
    }

    //Stay on position if target is reached
    if(state == IDLE)
        target = cmd;

    AccelServo::update(sinceLastCall);
}

void HomingServo::goTo(uint16_t value)
{
    if (1000 < value)
        value = 1000;

    switch(value)
    {
    case 0:
        state = HOMING_MIN;
        break;
    case 1000:
        state = HOMING_MAX;
        break;
    default:
        state = MOVING;
        break;
    }

    if(inverted)
        AccelServo::goTo(1000-value);
    else
        AccelServo::goTo(value);
}

bool HomingServo::isTargetReached()
{
    return state == IDLE;
}

uint16_t HomingServo::read()
{
	if(inverted)
		return 1000-AccelServo::read();
	else
		return AccelServo::read();
}
