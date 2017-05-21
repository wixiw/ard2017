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
    state = IP_RECAL_START_Y;
    status = InProgress;
    timeout.arm(10000);
    robot.nav.setPosition(525,800,90);
    robot.nav.recalFace(eTableBorder_TOP_Y, NO_ESCAPE);
    LOG_INFO("Install : Recaling on Referee border");
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
        LOG_ERROR("Install : Timeout");
    }

    switch (state) {
        case IP_IDLE:
            break;

        case IP_RECAL_START_Y:
            if(robot.nav.targetReached())
            {
                state = IP_MOVE_A_BIT;
                robot.nav.goTo(525, 730, eDir_BACKWARD);
                LOG_INFO("Install : Escaping RefereeBorder");
            }
            if(robot.nav.blocked())
            {
                state = IP_DONE;
                status = Failed;
                LOG_ERROR("Install : Recal failed");
            }
            break;

        case IP_MOVE_A_BIT:
            if(robot.nav.targetReached())
            {
                state = IP_RECAL_FLIP_FLOP;
                robot.nav.recalFace(eTableBorder_FLIP_FLOP_X, NO_ESCAPE);
                LOG_INFO("Install : Recal on Flip Flop");
            }
            if(robot.nav.blocked())
            {
                state = IP_DONE;
                status = Failed;
                LOG_ERROR("Install : Recal failed");
            }
            break;

        case IP_RECAL_FLIP_FLOP:
            if(robot.nav.targetReached())
            {
            	state = IP_DONE;
            	status = Success;
                LOG_INFO("Install : Ready");
            }
            if(robot.nav.blocked())
            {
                state = IP_DONE;
                status = Failed;
                LOG_ERROR("Install : Recal failed");
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
    state = IP_RECAL_START_Y;
    status = InProgress;
    robot.nav.setPosition(525,800,90);
    robot.nav.recalFace(eTableBorder_TOP_Y, NO_ESCAPE);
    LOG_INFO("Install : Recaling on Referee border");
}

void InstallTration::stop()
{
    state = IT_IDLE;
    status = None;
}

void InstallTration::update(TimeMs sinceLastCall)
{
    switch (state) {
        case IT_IDLE:
            break;

        case IP_RECAL_START_Y:
             if(robot.nav.targetReached())
             {
                 state = IT_MOVE_A_BIT;
                 robot.nav.goTo(525, 730, eDir_BACKWARD);
                 LOG_INFO("Install : Escaping RefereeBorder");
             }
             if(robot.nav.blocked())
             {
                 state = IT_DONE;
                 status = Failed;
                 LOG_ERROR("Install : Recal failed");
             }
             break;

         case IT_MOVE_A_BIT:
             if(robot.nav.targetReached())
             {
                 state = IT_RECAL_FLIP_FLOP;
                 robot.nav.recalFace(eTableBorder_FLIP_FLOP_X, 645 - TRATION_START_POSE.x);
                 LOG_INFO("Install : Recal on Flip Flop");
             }
             if(robot.nav.blocked())
             {
                 state = IT_DONE;
                 status = Failed;
                 LOG_ERROR("Install : Recal failed");
             }
             break;

         case IT_RECAL_FLIP_FLOP:
             if(robot.nav.targetReached())
             {
             	state = IT_WAIT_START;
             	robot.nav.goToCap(TRATION_START_POSE.x, TRATION_START_POSE.y - 300, -90, eDir_FORWARD);
                LOG_INFO("Install : Making room for Pen and wait start...");
             }
             if(robot.nav.blocked())
             {
                 state = IT_DONE;
                 status = Failed;
                 LOG_ERROR("Install : Recal failed");
             }
             break;

         case IT_WAIT_START:
             if(robot.hmi.isStartPlugged())
             {
                 state = IT_WAIT_START_WITHDRAW;
                 LOG_INFO("Install : Waiting Start withdraw");
             }
             break;

         case IT_WAIT_START_WITHDRAW:
             if(!robot.hmi.isStartPlugged())
             {
             	robot.nav.goToCap(TRATION_START_POSE, eDir_BACKWARD);
                 state = IT_GO_TO_START_POS;
                 LOG_INFO("Install : Positionning to start pose");
             }
             break;

         case IT_GO_TO_START_POS:
             if(robot.nav.targetReached())
             {
                 state = IT_DONE;
                 status = Success;
                 LOG_INFO("Install : Ready.");
             }
             if(robot.nav.blocked())
             {
                 state = IT_DONE;
                 status = Failed;
             }
             break;

         case IT_DONE:
        	status = Success;
            break;

        default:
            ASSERT(false);
            break;
    }
}
