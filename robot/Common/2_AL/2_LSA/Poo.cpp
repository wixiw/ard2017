/*
 * LSAPoo.cpp
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#include "Poo.h"
using namespace ard;

LSA_PooMiddle::LSA_PooMiddle(Robot2017& robot, eLSA_PooMidArea type):
        LSA2017(robot, "LSAPooMiddle"),
        type(type)
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);

    switch (type) {
        case Opp:
            setEntryPoint(Pose2D(-756, -234,  -45));
            break;
        case Center:
            setEntryPoint(Pose2D(   0,   70,  -90));
            break;
        case Own:
            setEntryPoint(Pose2D( 756, -234, -135));
            break;
        default:
            break;
    }
}

StrategyResult LSA_PooMiddle::getStatus()
{
    if(!fsm.isActive())
        return None;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

void LSA_PooMiddle::informPoo_generic()
{
    switch (type) {
        case Opp:
            informPooed_2(1);
            break;
        case Center:
            informPooed_3(1);
            break;
        case Own:
            informPooed_4(1);
            break;
        default:
            break;
    }
}

void LSA_PooMiddle::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}

DelayMs LSA_PooMiddle::getNominalDuration()
{
    return 7000;
}

/**
 * ------------------------
 */

LSA_PooBorderOwn::LSA_PooBorderOwn(Robot2017& robot):
        LSA2017(robot, "LSAPooMiddle", POO_BORDER_5_EP) //See RobotParameters.h
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);
}

StrategyResult LSA_PooBorderOwn::getStatus()
{
    if(!fsm.isActive())
        return None;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

void LSA_PooBorderOwn::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}

DelayMs LSA_PooBorderOwn::getNominalDuration()
{
    return 7000;
}

/**
 * ------------------------
 */
LSA_PooBorderOpp::LSA_PooBorderOpp(Robot2017& robot):
        LSA2017(robot, "LSAPooMiddle", Pose2D(0, 0, 0)) //TODO define real point
{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);
}

StrategyResult LSA_PooBorderOpp::getStatus()
{
    if(!fsm.isActive())
        return None;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

void LSA_PooBorderOpp::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}

DelayMs LSA_PooBorderOpp::getNominalDuration()
{
    return 7000;
}

/**
 * ------------------------
 */

LSA_PooStartArea::LSA_PooStartArea(Robot2017& robot):
//LSA2017(robot, "LSAPooMiddle", Pose2D(600, 760, -90)) //avec recalage
		LSA2017(robot, "LSAPooMiddle", Pose2D(545, 660, 115)) //sans recalage

{
    fsm.setTimer(&(robot.lifecycle.fsmTimer));
    fsm.setDefaultSCI_OCB(this);
}

StrategyResult LSA_PooStartArea::getStatus()
{
    if(!fsm.isActive())
        return None;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

void LSA_PooStartArea::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}

DelayMs LSA_PooStartArea::getNominalDuration()
{
    return 21000;
}

/**
 * ------------------------
 */
