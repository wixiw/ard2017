/*
 * Dispenser.cpp
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#include "Dispenser.h"
using namespace ard;

LSA_Dispenser::LSA_Dispenser(Robot2017& robot, eLSA_DispType type):
        LSA2017(robot, "LSADispenser"),
        type(type)
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

void LSA_Dispenser::informStratWithdraw()
{
    switch (type) {
        case Monocolor:
            informWithdraw_A(1);
            break;
        case Bicolor:
            informWithdraw_G(1);
            break;
        case OppBicolor:
            informWithdraw_OppG(1);
            break;
        default:
            break;
    }
}

sc_integer LSA_Dispenser::dispenserCount()
{
    sc_integer count;
    switch (type) {
        case Monocolor:
            count = dispenserA_Count();
            break;
        case Bicolor:
            count = dispenserG_Count();
            break;
        case OppBicolor:
            count = dispenserOppG_Count();
            break;
        default:
            break;
    }

    return count;
}
