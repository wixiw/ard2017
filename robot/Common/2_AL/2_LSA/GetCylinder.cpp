/*
 * GetCylinder.cpp
 *
 *  Created on: 16 avr. 2017
 *      Author: wix
 */

#include "GetCylinder.h"
using namespace ard;


LSA_GetCylinder_B::LSA_GetCylinder_B(Robot2017& robot, eLSA_GetCylinder_Recal type):
        LSA2017(robot, "LSAPooMiddle", PointCap(0, 0, 0)) //TODO define real point
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

void LSA_GetCylinder_B::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}


/**
 * ------------------------------
 */

LSA_GetCylinder_C::LSA_GetCylinder_C(Robot2017& robot, eLSA_GetCylinder_Recal type):
        LSA2017(robot, "LSAPooMiddle", PointCap(815, -650, -120))
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

void LSA_GetCylinder_C::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}


/**
 * ------------------------
 */


