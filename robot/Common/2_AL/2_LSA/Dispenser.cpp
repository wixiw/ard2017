/*
 * Dispenser.cpp
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#include "Dispenser.h"
using namespace ard;

String LSA_Dispenser::state2Str(FSM_LSA_Dispenser::FSM_LSA_DispenserStates state) const
{
    switch(state)
    {
    case FSM_LSA_Dispenser::main_region_Engage_dispenser:
        return "Engage_dispenser";
        break;
    case FSM_LSA_Dispenser::main_region_Get_1_cylinder:
        return "Get_1_cylinder";
        break;
    case FSM_LSA_Dispenser::main_region__final_:
        return "_final_";
        break;
    case FSM_LSA_Dispenser::main_region_Exit_dispenser:
        return "Exit_dispenser";
        break;
    default:
        return String("[LSA_Dispenser] unknown state (") + state + ")";
        break;
    }
}

LSA_Dispenser::LSA_Dispenser(Robot2017& robot, eLSA_DispType type):
        LSA2017(robot, "DispenserMonocolor")
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);

    switch (type) {
        case Monocolor:
            setEntryPoint(PointCap(  350,  650, 90));
            break;
        case Bicolor:
            setEntryPoint(PointCap( 1150, -350, 90));
            break;
        case OppBicolor:
            setEntryPoint(PointCap(-1150, -350, 90));
            break;
        default:
            break;
    }
}

LSAResult LSA_Dispenser::isFinished()
{
    if(!fsm.isActive())
        return NoLsa;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

void LSA_Dispenser::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}

