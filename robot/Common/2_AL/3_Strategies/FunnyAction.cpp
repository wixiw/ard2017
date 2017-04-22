/*
 * FunnyAction.cpp
 *
 *  Created on: 20 avr. 2017
 *      Author: wix
 */

#include "FunnyAction.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

FunnyAction::FunnyAction(Robot2017* robot):
        duration(0),
        robot(robot)
{
    ASSERT(robot);
}

void FunnyAction::update(TimeMs sinceLastCall)
{
    if( 1500 < duration )
    {
        //TODO 23° pour prise, à mettre dans installation
        robot->actuators.servoFunnyAction.write(180);
    }

    duration += sinceLastCall;
}

#endif
