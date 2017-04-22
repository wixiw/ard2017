/*
 * Strategies.h
 *
 *  Created on: 16 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_STRATEGIES_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_STRATEGIES_H_

#include "RSP.h"

#ifdef BUILD_STRATEGY

#include "2_LSA/Dispenser.h"
#include "2_LSA/GetCylinder.h"
#include "2_LSA/Poo.h"


#include "FunnyAction.h"
#include "Homol.h"
#include "Invade.h"
#include "Match.h"
#include "Installation.h"
#include "Selftest.h"
#include "WipPen.h"
#include "WipTration.h"


namespace ard
{
    class Robot2017;
    
    //Operational strategies
    void Strategy_Match(Robot2017& robot);
    void Strategy_Homol(Robot2017& robot);
    void Strategy_Invade(Robot2017& robot);
    void Strategy_Selftest(Robot2017& robot);

    //development in progress
    void Strategy_Tanguy(Robot2017& robot);
    void Strategy_Quentin(Robot2017& robot);

    //testing purposes
    void Strategy_LedTest(Robot2017& robot);
    void Strategy_ButtonTest(Robot2017& robot);
    void Strategy_OmronTest(Robot2017& robot);
    void Strategy_CalibRot(Robot2017& robot);
    void Strategy_CalibLin(Robot2017& robot);
    void Strategy_MotionTest(Robot2017& robot);
}

#endif //BUILD_STRATEGY

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_STRATEGIES_H_ */


