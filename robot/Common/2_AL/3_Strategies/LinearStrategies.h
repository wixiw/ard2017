/*
 * LinearStrategies.h
 *
 *  Created on: 26 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_3_STRATEGIES_LINEARSTRATEGIES_H_
#define ROBOT_COMMON_2_AL_3_STRATEGIES_LINEARSTRATEGIES_H_

#include "../Robot.h"

namespace ard
{
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

#endif /* ROBOT_COMMON_2_AL_3_STRATEGIES_LINEARSTRATEGIES_H_ */
