/*
 * GetCylinder.cpp
 *
 *  Created on: 16 avr. 2017
 *      Author: wix
 */

#include "GetCylinder.h"
using namespace ard;


String LSA_GetCylinder_B::state2Str(FSM_LSA_GetCylinder_B::FSM_LSA_GetCylinder_BStates state) const
{
    switch(state)
    {
    case FSM_LSA_GetCylinder_B::main_region_Engage_PooArea:
        return "Engage_PooArea";
        break;
    default:
        return String("[LSA_GetCylinder_B] unknown state (") + state + ")";
        break;
    }
}

LSA_GetCylinder_B::LSA_GetCylinder_B(Robot2017& robot, eLSA_GetCylinder_Recal type):
        LSA2017(robot, "LSAPooMiddle", PointCap(0, 0, 0)) //TODO define real point
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);
    fsm.set_recalRequested(type);
}

LSAResult LSA_GetCylinder_B::isFinished()
{
    if(!fsm.isActive())
        return NoLsa;

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

String LSA_GetCylinder_C::state2Str(FSM_LSA_GetCylinder_C::FSM_LSA_GetCylinder_CStates state) const
{
    switch(state)
    {
    case FSM_LSA_GetCylinder_C::main_region_Engage_PooArea:
        return "Engage_PooArea";
        break;
    default:
        return String("[LSA_GetCylinder_C] unknown state (") + state + ")";
        break;
    }
}

LSA_GetCylinder_C::LSA_GetCylinder_C(Robot2017& robot, eLSA_GetCylinder_Recal type):
        LSA2017(robot, "LSAPooMiddle", PointCap(0, 0, 0)) //TODO define real point
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);
}

LSAResult LSA_GetCylinder_C::isFinished()
{
    if(!fsm.isActive())
        return NoLsa;

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


