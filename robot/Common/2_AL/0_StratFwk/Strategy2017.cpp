/*
 * Strategy2017.cpp
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#include "Strategy2017.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"

using namespace ard;

Strategy2017::Strategy2017(Robot2017* _robot, String const& name):
        IStrategy(name),
        robot(*_robot)
{
    ASSERT_TEXT(_robot, "You did not attached a robot.");
}




#endif
