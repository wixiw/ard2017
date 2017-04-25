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

#include "3_Strategies/FunnyAction.h"
#include "3_Strategies/HomolPen.h"
#include "3_Strategies/HomolTration.h"
#include "3_Strategies/Installation.h"
#include "3_Strategies/Selftest.h"
#include "3_Strategies/WipPen.h"
#include "3_Strategies/WipTration.h"

namespace ard
{
    class Robot2017;
    
    //development in progress
    void Strategy_Tanguy(Robot2017& robot);

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


