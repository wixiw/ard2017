/*
 * Dispenser.cpp
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#include "Dispenser.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

FSM_LSA_Dispenser_Better::FSM_LSA_Dispenser_Better():
        lastState(FSM_LSA_Dispenser::FSM_LSA_Dispenser_last_state)
{

}

void FSM_LSA_Dispenser_Better::run()
{
    runCycle();
    FSM_LSA_DispenserStates newState = getState();
    if( lastState != newState)
    {
        LOG_INFO(String("[LSA_Dispenser] state changed :  ") + state2Str(lastState) + " => state " + state2Str(newState));
        lastState = newState;
    }
}

String FSM_LSA_Dispenser_Better::state2Str(FSM_LSA_DispenserStates state) const
{
    switch(state)
    {
    case main_region_Engage_dispenser:
        return "Engage_dispenser";
        break;
    case main_region_Get_1_cylinder:
        return "Get_1_cylinder";
        break;
    case main_region__final_:
        return "_final_";
        break;
    case main_region_Exit_dispenser:
        return "Exit_dispenser";
        break;
    default:
        return String("[LSA_Dispenser] unknown state (") + state + ")";
        break;
    }
}

LSA_Dispenser::LSA_Dispenser(Robot2017* robot, eLSA_DispType type):
        LSA(robot, "DispenserMonocolor")
{
    fsm.setTimer(&robot->fsmTimer);
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

void LSA_Dispenser::init()
{
    Action2017::init();
    fsm.init();
}

void LSA_Dispenser::start()
{
    fsm.init();
    fsm.enter();
}

void LSA_Dispenser::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
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

ACTION_2017_API_IMPL(LSA_Dispenser);

#endif
