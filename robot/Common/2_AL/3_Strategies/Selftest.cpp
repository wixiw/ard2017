/*
 * Selftest.cpp
 *
 *  Created on: 18 mai 2017
 *      Author: wix
 */

#include "Selftest.h"

using namespace ard;

Selftest::Selftest(Robot2017& robot)
        : IStrategy("Selftest"),
          state(ST_IDLE),
          robot(robot)
{
}

void Selftest::start()
{
    state = ST_OPEN_ARMS;
    status = InProgress;
    timeout.arm(10000);
    robot.actuators.servoLeftArm.goTo(1000);
    robot.actuators.servoRightArm.goTo(1000);
}

void Selftest::stop()
{
    state = ST_IDLE;
    status = None;
}

void Selftest::update(TimeMs sinceLastCall)
{
    if(timeout.isFired())
    {
        state = ST_DONE;
        status = Failed;
    }

    switch (state) {
        case ST_IDLE:
            break;

        case ST_OPEN_ARMS:
            if(robot.actuators.servoLeftArm.isTargetReached()
                    &&robot.actuators.servoRightArm.isTargetReached())
            {
                state = ST_LIFTER_UP;
                robot.actuators.lifterCmd(true);
            }
            break;

        case ST_LIFTER_UP:
            if(robot.actuators.servoLifter.isTargetReached())
            {
                state = ST_LIFTER_DOWN;
                robot.actuators.lifterCmd(false);
            }
            break;

        case ST_LIFTER_DOWN:
            if(robot.actuators.servoLifter.isTargetReached())
            {
                state = ST_LIFTER_UP;
                robot.actuators.lifterCmd(true);
            }
            break;

        case ST_LIFTER_UP2:
            if(robot.actuators.servoLifter.isTargetReached())
            {
                state = ST_TURN_WHEELS;
                robot.actuators.turnWheels(WC_SWALLOW);
                chrono.arm(2000);
            }
            break;

        case ST_TURN_WHEELS:
            if(chrono.isFired())
            {
                state = ST_STOP_WHEELS;
                robot.actuators.turnWheels(WC_IDLE);
                chrono.arm(500);
            }
            break;

        case ST_STOP_WHEELS:
            if(chrono.isFired())
            {
                state = ST_RETRACT;
                robot.actuators.servoLeftArm.goTo(0);
                robot.actuators.servoRightArm.goTo(0);
            }
            break;

        case ST_RETRACT:
            if(robot.actuators.servoLeftArm.isTargetReached()
                    &&robot.actuators.servoRightArm.isTargetReached())
            {
                state = ST_DONE;
                status = Success;
            }
            break;

        case ST_DONE:
            break;

        default:
            ASSERT(false);
            break;
    }
}

