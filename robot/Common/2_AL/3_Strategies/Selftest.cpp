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
    robot.actuators.servoLeftArm.goTo(750);
    robot.actuators.servoRightArm.goTo(750);
    robot.actuators.servoFunnyAction.goTo(FUNNY_REST);
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
                robot.actuators.servoLifter.goTo(LIFTER_MIN);
                robot.actuators.servo8.goTo(ROULEAU_RANGE);
            }
            break;

        case ST_LIFTER_UP:
            if(robot.actuators.servoLifter.isTargetReached())
            {
                state = ST_LIFTER_DOWN;
                robot.actuators.servoLifter.goTo(LIFTER_MAX);
            }
            break;

        case ST_LIFTER_DOWN:
            if(robot.actuators.servoLifter.isTargetReached())
            {
                state = ST_LIFTER_UP2;
                robot.actuators.servoLifter.goTo(LIFTER_MIN);
                robot.actuators.servo8.goTo(ROULEAU_SORTI);
            }
            break;

        case ST_LIFTER_UP2:
            if(robot.actuators.servoLifter.isTargetReached())
            {
                state = ST_TURN_WHEELS;
                robot.actuators.servoLeftWheel.goTo(1000);
                robot.actuators.servoRightWheel.goTo(1000);
                chrono.arm(1000);
            }
            break;

        case ST_TURN_WHEELS:
            if(chrono.isFired())
            {
                state = ST_STOP_WHEELS;
                robot.actuators.servoLeftWheel.goTo(500);
                robot.actuators.servoRightWheel.goTo(500);
                chrono.arm(500);
            }
            break;

        case ST_STOP_WHEELS:
            if(chrono.isFired())
            {
                state = ST_RETRACT;
                robot.actuators.servoLeftArm.goTo(ARM_MIN);
                robot.actuators.servoRightArm.goTo(ARM_MIN);
            }
            break;

        case ST_RETRACT:
            if(robot.actuators.servoLeftArm.isTargetReached()
                    &&robot.actuators.servoRightArm.isTargetReached())
            {
                state = ST_FUNNY_LAUNCH;
                robot.actuators.servoFunnyAction.goTo(FUNNY_LAUNCH);
                robot.actuators.servo8.goTo(ROULEAU_RANGE);
                chrono.arm(500);
            }
            break;

        case ST_FUNNY_LAUNCH:
            if(chrono.isFired())
            {
            	robot.actuators.servoFunnyAction.goTo(FUNNY_REST);
            	chrono.arm(500);
                state = ST_FUNNY_REST;
            }
            break;

        case ST_FUNNY_REST:
            if(chrono.isFired())
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

