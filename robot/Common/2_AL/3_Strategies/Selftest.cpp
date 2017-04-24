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
        lastState(FSM_Selftest_last_state)
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
            ASSERT(false);
            return "";
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
}

void Selftest::update(TimeMs sinceLastCall)
{
    if(!fsm.isActive())
        fsm.enter();

    fsm.run();

    robot.actuators.servoLeftArm.      write(fsm.getDefaultSCI()->get_leftArm());
    robot.actuators.servoRightArm.     write(fsm.getDefaultSCI()->get_rightArm());
    robot.actuators.servoLeftWheel.    write(fsm.getDefaultSCI()->get_leftWheel());
    robot.actuators.servoRightWheel.   write(fsm.getDefaultSCI()->get_rightWheel());
    robot.actuators.servoLifter.       write(fsm.getDefaultSCI()->get_lifter());
}

STRAT_2017_API_IMPL(Selftest);

#endif
