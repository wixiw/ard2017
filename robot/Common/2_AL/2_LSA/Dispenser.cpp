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
            setEntryPoint(Pose2D(  350,  715, 90));
            break;
        case Bicolor:
            setEntryPoint(Pose2D( 1230, -350, 0)); // TODO recaler
            break;
        case OppBicolor:
            setEntryPoint(Pose2D(-1230, -350, 180)); // TODO recaler
            break;
        default:
            break;
    }
}

StrategyResult LSA_Dispenser::getStatus()
{
    if(!fsm.isActive())
        return None;

    if(fsm.isFinal())
        return Success;

    return InProgress;
}

void LSA_Dispenser::goToEntryPoint()
{
    robot.nav.goToCap(getEntryPoint(), eDir_BACKWARD);
}

DelayMs LSA_Dispenser::getNominalDuration()
{
    return 18000;
}

void LSA_Dispenser::informWithdraw_generic()
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

sc_integer LSA_Dispenser::howManyToTake()
{
	//Cylinders to take is the min between :
	//the number of space available in the robot
	//the number of cylinder in the dispenser
	//the number of cylinder withdraw requested by the strategy
	return min(robot.stratInfo.getFreeStockRoom(), min(param, dispenserCount()));
}

sc_boolean LSA_Dispenser::shouldISlowOnNextCylinder()
{
	return (dispenserCount() == 2);
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
