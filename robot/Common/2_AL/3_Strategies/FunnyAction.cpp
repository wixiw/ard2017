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
	//First try
    if( 1000 < duration && duration <= 1500 )
    {
    	LOG_INFO("[Funny] Launch 1");
        robot.actuators.servoFunnyAction.goTo(FUNNY_LAUNCH);
    }
    if( 1500 < duration && duration <= 2000 )
        robot.actuators.servoFunnyAction.goTo(FUNNY_REST);

    //Second try
    if( 2000 < duration && duration <= 2500 )
    {
    	LOG_INFO("[Funny] Launch 2");
        robot.actuators.servoFunnyAction.goTo(FUNNY_LAUNCH);
    }
    if( 2500 < duration && duration <= 3000 )
        robot.actuators.servoFunnyAction.goTo(FUNNY_REST);

    //Third try
    if( 3000 < duration && duration <= 3500 )
    {
    	LOG_INFO("[Funny] Launch 3");
        robot.actuators.servoFunnyAction.goTo(FUNNY_LAUNCH);
    }
    if( 3500 < duration && duration <= 4000 )
        robot.actuators.servoFunnyAction.goTo(FUNNY_REST);

    //Stop before end to be sure to not not show if our match timer is not precise ^^
    if( 4000 < duration )
    {
        robot.actuators.servoFunnyAction.disable();
        status = Success;
    }

    duration += sinceLastCall;
}
