/*
 * LSA_GetCylinder.cpp
 *
 *  Created on: 16 avr. 2017
 *      Author: wix
 */

#include "LSA_GetCylinder.h"
#include "../Robot2017.h"

using namespace ard;

/**
 *  ----  LSA_GetCylinderCraters  ----
 */

LSA_GetCylinderCraters::LSA_GetCylinderCraters(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

LSA_result LSA_GetCylinderCraters::execute(uint8_t nbItems)
{
    LOG_INFO("LSA_GetCylinderCraters success");
    return LSA_Success;
}

/**
 *  ----  LSA_GetCylinderCorner  ----
 */

LSA_GetCylinderCorner::LSA_GetCylinderCorner(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

LSA_result LSA_GetCylinderCorner::execute(uint8_t nbItems)
{
    LOG_INFO("LSA_GetCylinderCorner success");
    return LSA_Success;
}

/**
 *  ----  LSA_GetCylinderOpp  ----
 */

LSA_GetCylinderOpp::LSA_GetCylinderOpp(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

LSA_result LSA_GetCylinderOpp::execute(uint8_t nbItems)
{
    LOG_INFO("LSA_GetCylinderCraters success");
    return LSA_Success;
}
