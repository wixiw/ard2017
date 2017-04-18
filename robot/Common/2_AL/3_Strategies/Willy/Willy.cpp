/*
 * Willy.cpp
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#include "Willy.h"

using namespace ard;

Willy::Willy(TimerInterface& timer):
        fsm()
{
    fsm.setTimer(&timer);
}

void Willy::init()
{
    fsm.init();
}

void Willy::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}
