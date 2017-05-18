/*
 * Installation.cpp
 *
 *  Created on: 18 mai 2017
 *      Author: wix
 */

#include "Installation.h"

using namespace ard;


InstallPen::InstallPen(Robot2017& robot)
        : IStrategy("InstallPen"),
          state(IP_IDLE),
          robot(robot)
{
}

void InstallPen::start()
{
    state = IP_RECAL_FLIP_FLOP;
    status = InProgress;
    timeout.arm(10000);
    robot.nav.setPosition(555,730,180);
    robot.nav.recalRear(eTableBorder_FLIP_FLOP_X);
}

void InstallPen::stop()
{
    state = IP_IDLE;
    status = None;
}

void InstallPen::update(TimeMs sinceLastCall)
{
    if(timeout.isFired())
    {
        state = IP_DONE;
        status = Failed;
    }

    switch (state) {
        case IP_IDLE:
            break;

        case IP_RECAL_FLIP_FLOP:
            if(robot.nav.targetReached())
            {
                state = IP_RECAL_START_Y;
                robot.nav.recalRear(eTableBorder_TOP_Y);
            }
            if(robot.nav.blocked())
            {
                state = IP_DONE;
                status = Failed;
            }
            break;

        case IP_RECAL_START_Y:
            if(robot.nav.targetReached())
            {
                state = IP_MOVE_A_BIT;
                robot.nav.goTo(590, 730, eDir_BACKWARD);
            }
            if(robot.nav.blocked())
            {
                state = IP_DONE;
                status = Failed;
            }
            break;

        case IP_MOVE_A_BIT:
            if(robot.nav.targetReached())
            {
                state = IP_GO_TO_START_POS;
                robot.nav.goTo(640, 730, eDir_FORWARD);
            }
            if(robot.nav.blocked())
            {
                state = IP_DONE;
                status = Failed;
            }
            break;

        case IP_GO_TO_START_POS:
            if(robot.nav.targetReached())
            {
                state = IP_DONE;
                status = Success;
            }
            if(robot.nav.blocked())
            {
                state = IP_DONE;
                status = Failed;
            }
            break;

        case IP_DONE:
            break;

        default:
            ASSERT(false);
            break;
    }
}

InstallTration::InstallTration(Robot2017& robot)
        : IStrategy("InstallTration"),
          state(IT_IDLE),
          robot(robot)
{
}

void InstallTration::start()
{
    state = IT_DONE;
    status = InProgress;
    timeout.arm(10000);
    robot.actuators.servoLeftArm.goTo(1000);
    robot.actuators.servoRightArm.goTo(1000);
}

void InstallTration::stop()
{
    state = IT_IDLE;
    status = None;
}

void InstallTration::update(TimeMs sinceLastCall)
{
    if(timeout.isFired())
    {
        state = IT_DONE;
        status = Failed;
    }

    switch (state) {
        case IT_IDLE:
            break;

        case IT_DONE:
            break;

        default:
            ASSERT(false);
            break;
    }
}
