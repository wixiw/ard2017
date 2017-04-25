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
        default:
            return String("[LSA_Dispenser] unknown state (") + state + ")";
            break;
    }
}

LSA_Dispenser::LSA_Dispenser(Robot2017* robot, eLSA_DispType type):
        LSA(robot, "DispenserMonocolor")
{
    //fsm.setTimer(&robot->fsmTimer);
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

//    LOG_INFO("Engage Monocolor Dispenser");
//    robot.nav.goToCap(350, 770, 90);
//    robot.nav.wait();
//
//    LOG_INFO("Get 4 Cylinders from Monocolor dispenser");
//
//    if( nbItems == 0 )
//        nbItems = robot.strategy.getStratInfo().dispenserMonocolorNb;
//
//    for(int i = nbItems ; 0 < i ; i--)
//    {
//        ArdOs::sleepMs(500);
//        robot.strategy.informWithdraw_MonocolorDispenser(1);
//    }
//
//    LOG_INFO("Leave Monocolor Dispenser");
//    robot.nav.goTo(entryPoint, eDir_BACKWARD);
//    robot.nav.wait();
//
//    LOG_INFO("Dispenser_Monocolor success");
//    return Success;

void LSA_Dispenser::init()
{
    Action2017::init();
    fsm.init();
    fsm.enter();
}

void LSA_Dispenser::start()
{
    //reset FSM and reset status
    //TODO
}

void LSA_Dispenser::update(TimeMs sinceLastCall)
{
    fsm.runCycle();
}

LSAResult LSA_Dispenser::isFinished() const
{
    //TODO
    return Success;
}

ACTION_2017_API_IMPL(LSA_Dispenser);

#endif
