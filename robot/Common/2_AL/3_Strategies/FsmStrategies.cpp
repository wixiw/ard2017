/*
 * FsmStrategies.cpp
 *
 *  Created on: 26 avr. 2017
 *      Author: wix
 */

#include "FsmStrategies.h"
using namespace ard;


String Selftest::state2Str(FSM_Selftest::FSM_SelftestStates state) const
{
    switch(state)
    {
        case FSM_Selftest::FSM_Selftest_last_state:
            return "init state";
            break;
        default:
            return String("[Selftest] unknown state (") + state + ")";
            break;
    }
}

String HomolPen::state2Str(FSM_HomolPen::FSM_HomolPenStates state) const
{
    switch(state)
    {
        case FSM_HomolPen::FSM_HomolPen_last_state:
            return "init state";
            break;
        default:
            return String("[HomolPen] unknown state (") + state + ")";
            break;
    }
}

String HomolTration::state2Str(FSM_HomolTration::FSM_HomolTrationStates state) const
{
    switch(state)
    {
        case FSM_HomolTration::FSM_HomolTration_last_state:
            return "init state";
            break;
        default:
            return String("[HomolTration] unknown state (") + state + ")";
            break;
    }
}

String InstallPen::state2Str(FSM_InstallPen::FSM_InstallPenStates state) const
{
    switch(state)
    {
        case FSM_InstallPen::FSM_InstallPen_last_state:
            return "init state";
            break;
        default:
            return String("[InstallPen] unknown state (") + state + ")";
            break;
    }
}

String InstallTration::state2Str(FSM_InstallTration::FSM_InstallTrationStates state) const
{
    switch(state)
    {
        case FSM_InstallTration::FSM_InstallTration_last_state:
            return "init state";
            break;
        default:
            return String("[InstallTration] unknown state (") + state + ")";
            break;
    }
}

String WipPen::state2Str(FSM_WipPen::FSM_WipPenStates state) const
{
    switch(state)
    {
        case FSM_WipPen::FSM_WipPen_last_state:
            return "init state";
            break;
        default:
            return String("[WipPen] unknown state (") + state + ")";
            break;
    }
}

String WipTration::state2Str(FSM_WipTration::FSM_WipTrationStates state) const
{
    switch(state)
    {
        case FSM_WipTration::FSM_WipTration_last_state:
            return "init state";
            break;
        default:
            return String("[WipTration] unknown state (") + state + ")";
            break;
    }
}
