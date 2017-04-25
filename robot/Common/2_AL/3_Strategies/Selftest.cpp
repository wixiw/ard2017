/*
 * Selftest.cpp
 *
 *  Created on: 21 avr. 2017
 *      Author: wix
 */

#include "Selftest.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"
using namespace ard;

FSM_Selftest_Better::FSM_Selftest_Better():
        lastState(FSM_Selftest::FSM_Selftest_last_state)
{

}

void FSM_Selftest_Better::run()
{
    runCycle();
    FSM_SelftestStates newState = getState();
    if( lastState != newState)
    {
        LOG_INFO(String("[Selftest] state changed :  ") + state2Str(lastState) + " => state " + state2Str(newState));
        lastState = newState;
    }
}

String FSM_Selftest_Better::state2Str(FSM_SelftestStates state) const
{
    switch(state)
    {
        case main_region_StateA:
            return "main_region_StateA";
            break;
        case main_region_StateB:
            return "main_region_StateB";
            break;
        case FSM_Selftest_last_state:
            return "init state";
            break;
        default:
            return String("[Selftest] unknown state (") + state + ")";
            break;
    }
}

Selftest::Selftest(Robot2017* robot):
        Strategy2017(robot, "Selftest"),
        fsm()
{
    fsm.setTimer(&robot->fsmTimer);
    fsm.setDefaultSCI_OCB(this);
}

void Selftest::init()
{
    Strategy2017::init();
    fsm.init();
    fsm.enter();
}

void Selftest::update(TimeMs sinceLastCall)
{
    Strategy2017::update(sinceLastCall);
    fsm.run();
}

STRAT_2017_API_IMPL(Selftest);

#endif
