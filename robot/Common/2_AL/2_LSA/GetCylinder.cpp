/*
 * GetCylinder.cpp
 *
 *  Created on: 16 avr. 2017
 *      Author: wix
 */

#include "GetCylinder.h"
#include "Robot2017.h"

using namespace ard;

/**
 *  ----  GetCylinderCraters  ----
 */

GetCylinderCraters::GetCylinderCraters(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

result GetCylinderCraters::execute(uint8_t nbItems)
{
    LOG_INFO("GetCylinderCraters success");
    return Success;
}

/**
 *  ----  GetCylinderCorner  ----
 */

GetCylinderCorner::GetCylinderCorner(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

result GetCylinderCorner::execute(uint8_t nbItems)
{
    LOG_INFO("GetCylinderCorner success");
    return Success;
}

/**
 *  ----  GetCylinderOpp  ----
 */

GetCylinderOpp::GetCylinderOpp(Robot2017& robot):
                LSA(robot, PointCap(0, 0, 0))
{
}

result GetCylinderOpp::execute(uint8_t nbItems)
{
    LOG_INFO("GetCylinderCraters success");
    return Success;
}
