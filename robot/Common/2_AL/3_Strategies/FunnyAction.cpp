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
<<<<<<< HEAD
}

void FunnyAction::start()
{

=======
>>>>>>> 47c8f5df3f245d1a6d1a376ebf746dc9ee2e0a2c
}

void FunnyAction::update(TimeMs sinceLastCall)
{
    if( 1500 < duration )
    {
        //TODO 23° pour prise, à mettre dans installation
        robot.actuators.servoFunnyAction.write(1000);
    }

    if( 4500 < duration )
    {
        robot.actuators.servoFunnyAction.disable();
    }

    duration += sinceLastCall;
}
