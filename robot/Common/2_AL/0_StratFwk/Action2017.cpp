/*
 * Strategy2017.cpp
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#include "Action2017.h"
#ifdef BUILD_STRATEGY

using namespace ard;

Action2017::Action2017(Robot2017* _robot, String const& name):
        PolledObject(name),
        robot(*_robot)
{
    ASSERT_TEXT(_robot, "You did not attached a robot.");
}

#endif
