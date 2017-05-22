/*
 * FunnyAction.cpp
 *
 *  Created on: 20 avr. 2017
 *      Author: wix
 */

#include "FunnyAction.h"
using namespace ard;

FunnyAction::FunnyAction(Robot2017& robot):
        IStrategy("FunnyAction"),
        duration(0),
        robot(robot)
{
}

void FunnyAction::start()
{
}

void FunnyAction::update(TimeMs sinceLastCall)
{
    if( 1000 < duration && duration <= 1500 )
        robot.actuators.servoFunnyAction.goTo(FUNNY_PUSH);
    if( 1500 < duration && duration <= 2000 )
        robot.actuators.servoFunnyAction.goTo(FUNNY_REST);
    if( 2000 < duration && duration <= 2500 )
        robot.actuators.servoFunnyAction.goTo(FUNNY_PUSH);
    if( 2500 < duration && duration <= 3000 )
        robot.actuators.servoFunnyAction.goTo(FUNNY_REST);
    if( 3000 < duration && duration <= 3500 )
        robot.actuators.servoFunnyAction.goTo(FUNNY_PUSH);
    if( 3500 < duration && duration <= 4000 )
        robot.actuators.servoFunnyAction.goTo(FUNNY_REST);

    if( 4000 < duration )
    {
        robot.actuators.servoFunnyAction.disable();
        status = Success;
    }

    duration += sinceLastCall;
}
