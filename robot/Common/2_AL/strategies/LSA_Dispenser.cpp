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

LSA_Dispenser_Monocolor::LSA_Dispenser_Monocolor(Robot2017& robot):
        LSA(robot, PointCap(350, 700, 90))
{
}

LSA_result LSA_Dispenser_Monocolor::execute(uint8_t nbItems)
{
    LOG_INFO("Engage Monocolor Dispenser");
    robot.nav.goToCap(350, 770, 90);
    robot.nav.wait();

    LOG_INFO("Get 4 Cylinders from Monocolor dispenser");
    ArdOs::sleepMs(2000);

    robot.strategy.informWithdraw_MonocolorDispenser(nbItems);

    LOG_INFO("Leave Monocolor Dispenser");
    robot.nav.goTo(entryPoint, eDir_BACKWARD);
    robot.nav.wait();

    LOG_INFO("LSA_Dispenser_Monocolor success");
    return LSA_Success;
}

/**
 *  ----  LSA_Dispenser_Bicolor  ----
 */


LSA_Dispenser_Bicolor::LSA_Dispenser_Bicolor(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

LSA_result LSA_Dispenser_Bicolor::execute(uint8_t nbItems)
{
    LOG_INFO("LSA_Dispenser_Bicolor success");
    return LSA_Success;
}

/**
 *  ----  LSA_Dispenser_Opp  ----
 */


LSA_Dispenser_Opp::LSA_Dispenser_Opp(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

LSA_result LSA_Dispenser_Opp::execute(uint8_t nbItems)
{
    LOG_INFO("LSA_Dispenser_Opp success");
    return LSA_Success;
}
