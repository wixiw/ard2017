/*
 * LinearStrategies.h
 *
 *  Created on: 26 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_3_STRATEGIES_LINEARSTRATEGIES_H_
#define ROBOT_COMMON_2_AL_3_STRATEGIES_LINEARSTRATEGIES_H_

#include "Robot2017.h"

namespace ard
{
    //development in progress
    void Strategy_Tanguy(void* robot);

    //testing purposes
    void Strategy_LedTest(void* robot);
    void Strategy_ButtonTest(void* robot);
    void Strategy_OmronTest(void* robot);
    void Strategy_CalibRot(void* robot);
    void Strategy_CalibLin(void* robot);
    void Strategy_MotionTest(void* robot);
}

#endif /* ROBOT_COMMON_2_AL_3_STRATEGIES_LINEARSTRATEGIES_H_ */
