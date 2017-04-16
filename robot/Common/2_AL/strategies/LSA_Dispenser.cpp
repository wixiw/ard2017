/*
 * LSA_Dispenser.cpp
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#include "LSA_Dispenser.h"
#include "../Robot2017.h"

using namespace ard;

/**
 *  ----  LSA_Dispenser_Monocolor  ----
 */

LSA_DispenserMonocolor::LSA_DispenserMonocolor(Robot2017& robot):
        LSA(robot, PointCap(350, 700, 90))
{
}

LSA_result LSA_DispenserMonocolor::execute(uint8_t nbItems)
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
        robot.strategy.informWithdraw_MonocolorDispenser(1);
    }

    LOG_INFO("Leave Monocolor Dispenser");
    robot.nav.goTo(entryPoint, eDir_BACKWARD);
    robot.nav.wait();

    LOG_INFO("LSA_Dispenser_Monocolor success");
    return LSA_Success;
}

/**
 *  ----  LSA_DispenserBicolor  ----
 */


LSA_DispenserBicolor::LSA_DispenserBicolor(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

LSA_result LSA_DispenserBicolor::execute(uint8_t nbItems)
{
    LOG_INFO("LSA_DispenserBicolor success");
    return LSA_Success;
}

/**
 *  ----  LSA_DispenserOpp  ----
 */


LSA_DispenserOpp::LSA_DispenserOpp(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

LSA_result LSA_DispenserOpp::execute(uint8_t nbItems)
{
    LOG_INFO("LSA_DispenserOpp success");
    return LSA_Success;
}
