/*
 * Installation.cpp
 *
 *  Created on: 18 mai 2017
 *      Author: wix
 */

#include "Installation.h"

using namespace ard;

#define INSTALL_VMAX 300 		/*mm/s*/
#define INSTALL_AMAX 300		/*°/s2*/
#define INSTALL_TURN_VMAX 125 	/*°/s*/
#define INSTALL_TURN_AMAX 80 	/*mm/s2*/

InstallPen::InstallPen(Robot2017& robot)
        : IStrategy("InstallPen"),
          state(IP_IDLE),
          robot(robot)
{
}

void InstallPen::start()
{
    state = IP_PREPARE_FLIPFLOP;
    status = InProgress;
    timeout.arm(25000);
    robot.nav.setPosition(PEN_INSTALL_POSE);
    robot.kinematics.setSpeedAcc(INSTALL_VMAX , INSTALL_TURN_VMAX, INSTALL_AMAX, INSTALL_TURN_AMAX);
    robot.nav.goForward(50);
    LOG_INFO("Install : Go forward a bit to prepare fli-flop recal.");
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

        case IP_PREPARE_FLIPFLOP:
            if(robot.nav.targetReached())
            {
                state = IP_RECAL_FLIP_FLOP;
                robot.nav.recalRear(eTableBorder_FLIP_FLOP_X, 200);
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
            	state = IP_RECAL_REFEREE_Y;
                robot.nav.recalFace(eTableBorder_REFEREE_Y, NO_ESCAPE);
                LOG_INFO("Install : Recaling on Referee border");
            }
            if(robot.nav.blocked())
            {
                state = IP_DONE;
                status = Failed;
                LOG_ERROR("Install : Recal failed");
            }
            break;

        case IP_RECAL_REFEREE_Y:
            if(robot.nav.targetReached())
            {
                state = IP_ESCAPE_REFEREE;
                robot.nav.goForward(-(1000 - robot.conf.xav() - PEN_START_POSE.y));
                LOG_INFO("Install : Escaping RefereeBorder");
            }
            if(robot.nav.blocked())
            {
                state = IP_DONE;
                status = Failed;
                LOG_ERROR("Install : Recal failed");
            }
            break;

        case IP_ESCAPE_REFEREE:
            if(robot.nav.targetReached())
            {
                state = IP_GO_TO_START_POS;
                robot.nav.goToCap(PEN_START_POSE, eDir_BACKWARD);
                LOG_INFO("Install : Positionning in start area");
            }
            if(robot.nav.blocked())
            {
                state = IP_DONE;
                status = Failed;
                LOG_ERROR("Install : Recal failed");
            }
            break;

        case IP_GO_TO_START_POS:
            if(robot.nav.targetReached())
            {
            	status = Success;
            	robot.kinematics.removeUserConstraints();
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
        	//Do nothing here as eventually, the next strategy will be choosen
        	//before entering the state in this one as status==Success
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
    robot.nav.setPosition(TRATION_INSTALL_POSE);
    robot.kinematics.setSpeedAcc(INSTALL_VMAX , INSTALL_TURN_VMAX, INSTALL_AMAX, INSTALL_TURN_AMAX);
    robot.nav.recalFace(eTableBorder_REFEREE_Y, NO_ESCAPE);
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
                 robot.nav.goTo(TRATION_INSTALL_POSE.x, 730, eDir_BACKWARD);
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
                 robot.nav.recalRear(eTableBorder_FLIP_FLOP_X, 790 - robot.conf.xar() - TRATION_START_POSE.x);
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
             	state = IT_ESCAPING_FOR_PEN;
             	robot.nav.goToCap(TRATION_START_POSE.x, 400+30 /*cylinder D outer*/ + robot.conf.xav() + 10 /*"Margin"*/, -90, eDir_FORWARD);
                LOG_INFO("Install : Making room for Pen and wait start...");
             }
             if(robot.nav.blocked())
             {
                 state = IT_DONE;
                 status = Failed;
                 LOG_ERROR("Install : Recal failed");
             }
             break;

         case IT_ESCAPING_FOR_PEN:
             if(robot.nav.targetReached())
             {
             	state = IT_WAIT_START;
                LOG_INFO("Install : Waiting start installation...");
                robot.hmi.buzzer.bip(2);
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
            	 robot.hmi.buzzer.bip(1);
                 state = IT_WAIT_START_WITHDRAW;
                 LOG_INFO("Install : Waiting Start withdraw");
             }
             break;

         case IT_WAIT_START_WITHDRAW:
             if(!robot.hmi.isStartPlugged())
             {
            	 robot.hmi.buzzer.bip(1);
            	 robot.nav.goToCap(TRATION_START_POSE, eDir_BACKWARD);
                 state = IT_GO_TO_START_POS;
                 LOG_INFO("Install : Positionning to start pose");
             }
             break;

         case IT_GO_TO_START_POS:
             if(robot.nav.targetReached())
             {
            	 robot.hmi.buzzer.bip(2);
                 state = IT_DONE;
                 status = Success;
                 robot.kinematics.removeUserConstraints();
                 LOG_INFO("Install : Ready.");
             }
             if(robot.nav.blocked())
             {
                 state = IT_DONE;
                 status = Failed;
             }
             break;

         case IT_DONE:
         	//Do nothing here as eventually, the next strategy will be choosen
         	//before entering the state in this one as status==Success
            break;

        default:
            ASSERT(false);
            break;
    }
}
