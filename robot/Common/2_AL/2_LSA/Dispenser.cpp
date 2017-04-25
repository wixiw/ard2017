/*
 * Dispenser.cpp
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#include "Dispenser.h"
#include "Robot2017.h"
#ifdef BUILD_STRATEGY
using namespace ard;

/**
 *  ----  Dispenser_Monocolor  ----
 */

DispenserMonocolor::DispenserMonocolor(Robot2017& robot):
        LSA(robot, PointCap(350, 700, 90))
{
}

LSAResult DispenserMonocolor::execute(uint8_t nbItems)
{
    LOG_INFO("Engage Monocolor Dispenser");
    robot.nav.goToCap(350, 770, 90);
    robot.nav.wait();

    LOG_INFO("Get 4 Cylinders from Monocolor dispenser");

    if( nbItems == 0 )
        nbItems = robot.strategy.getStratInfo().dispenserMonocolorNb;

    for(int i = nbItems ; 0 < i ; i--)
    {
        ArdOs::sleepMs(500);
        robot.strategy.informWithdraw_A(1);
    }

    LOG_INFO("Leave Monocolor Dispenser");
    robot.nav.goTo(entryPoint, eDir_BACKWARD);
    robot.nav.wait();

    LOG_INFO("Dispenser_Monocolor success");
    return Success;
}

/**
 *  ----  DispenserBicolor  ----
 */


DispenserBicolor::DispenserBicolor(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

LSAResult DispenserBicolor::execute(uint8_t nbItems)
{
    LOG_INFO("DispenserBicolor success");
    return Success;
}

/**
 *  ----  DispenserOpp  ----
 */


DispenserOpp::DispenserOpp(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

LSAResult DispenserOpp::execute(uint8_t nbItems)
{
    LOG_INFO("DispenserOpp success");
    return Success;
}

#endif