/*
 * LSAPoo.cpp
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#include "Poo.h"
using namespace ard;

String LSA_PooMiddle::state2Str(FSM_LSA_PooMiddle::FSM_LSA_PooMiddleStates state) const
{
    switch(state)
    {
    case FSM_LSA_PooMiddle::main_region_Engage_PooArea:
        return "Engage_PooArea";
        break;
    case FSM_LSA_PooMiddle::main_region_Put_1_cylinder:
        return "Put_1_cylinder";
        break;
    case FSM_LSA_PooMiddle::main_region__final_:
        return "_final_";
        break;
    case FSM_LSA_PooMiddle::main_region_Exit_Poo_Area:
        return "Exit_Poo_Area";
        break;
    default:
        return String("[LSA_PooMiddle] unknown state (") + state + ")";
        break;
    }
}

LSA_PooMiddle::LSA_PooMiddle(Robot2017& robot, eLSA_PooMidArea type):
        LSA2017(robot, "LSAPooMiddle")
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);

    switch (type) {
        case Opp:
            setEntryPoint(PointCap(-756, -244,  -45));
            break;
        case Center:
            setEntryPoint(PointCap(   0,   70,  -90));
            break;
        case Own:
            setEntryPoint(PointCap( 756, -244, -135));
            break;
        default:
            break;
    }
}

LSAResult LSA_PooMiddle::isFinished()
{
    if(!fsm.isActive())
        return NoLsa;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

void LSA_PooMiddle::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}

/**
 * ------------------------
 */

String LSA_PooBorderOwn::state2Str(FSM_LSA_PooBorderOwn::FSM_LSA_PooBorderOwnStates state) const
{
    switch(state)
    {
    case FSM_LSA_PooBorderOwn::main_region_Engage_PooArea:
        return "Engage_PooArea";
        break;
    default:
        return String("[LSA_PooBorderOwn] unknown state (") + state + ")";
        break;
    }
}

LSA_PooBorderOwn::LSA_PooBorderOwn(Robot2017& robot):
        LSA2017(robot, "LSAPooMiddle", PointCap(0, 0, 0)) //TODO define real point
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);
}

LSAResult LSA_PooBorderOwn::isFinished()
{
    if(!fsm.isActive())
        return NoLsa;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

void LSA_PooBorderOwn::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}

/**
 * ------------------------
 */

String LSA_PooBorderOpp::state2Str(FSM_LSA_PooBorderOpp::FSM_LSA_PooBorderOppStates state) const
{
    switch(state)
    {
    case FSM_LSA_PooBorderOpp::main_region_Engage_PooArea:
        return "Engage_PooArea";
        break;
    default:
        return String("[LSA_PooBorderOpp] unknown state (") + state + ")";
        break;
    }
}

LSA_PooBorderOpp::LSA_PooBorderOpp(Robot2017& robot):
        LSA2017(robot, "LSAPooMiddle", PointCap(0, 0, 0)) //TODO define real point
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);
}

LSAResult LSA_PooBorderOpp::isFinished()
{
    if(!fsm.isActive())
        return NoLsa;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

void LSA_PooBorderOpp::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}

/**
 * ------------------------
 */

String LSA_PooStartArea::state2Str(FSM_LSA_PooStartArea::FSM_LSA_PooStartAreaStates state) const
{
    switch(state)
    {
    case FSM_LSA_PooStartArea::main_region_Engage_PooArea:
        return "Engage_PooArea";
        break;
    default:
        return String("[LSA_PooStartArea] unknown state (") + state + ")";
        break;
    }
}

LSA_PooStartArea::LSA_PooStartArea(Robot2017& robot):
        LSA2017(robot, "LSAPooMiddle", PointCap(0, 0, 0)) //TODO define real point
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);
}

LSAResult LSA_PooStartArea::isFinished()
{
    if(!fsm.isActive())
        return NoLsa;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

void LSA_PooStartArea::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}

/**
 * ------------------------
 */
