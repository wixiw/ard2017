/*
 * GetCylinder.cpp
 *
 *  Created on: 16 avr. 2017
 *      Author: wix
 */

#include "GetCylinder.h"
using namespace ard;


LSA_GetCylinder_B::LSA_GetCylinder_B(Robot2017& robot, eLSA_GetCylinder_Recal type):
        LSA2017(robot, "LSAPooMiddle", PointCap(1016,116, 45))
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);
    fsm.set_recalRequested(type);
}

StrategyResult LSA_GetCylinder_B::getStatus()
{
    if(!fsm.isActive())
        return None;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

DelayMs LSA_GetCylinder_B::getNominalDuration()
{
    return 10000;
}

void LSA_GetCylinder_B::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}


/**
 * ------------------------------
 */

LSA_GetCylinder_C::LSA_GetCylinder_C(Robot2017& robot, eLSA_GetCylinder_Recal type):
        LSA2017(robot, "LSAPooMiddle", PointCap(780, -600, -108))
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);
}

StrategyResult LSA_GetCylinder_C::getStatus()
{
    if(!fsm.isActive())
        return None;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

DelayMs LSA_GetCylinder_C::getNominalDuration()
{
    return 7000;
}

void LSA_GetCylinder_C::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}


/**
 * ------------------------
 */


