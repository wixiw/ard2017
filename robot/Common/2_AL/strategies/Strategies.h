/*
 * Strategies.h
 *
 *  Created on: 16 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_STRATEGIES_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_STRATEGIES_H_

#include "LSA_Dispenser.h"
#include "LSA_GetCylinder.h"
#include "LSA_Poo.h"

namespace ard
{
    class Robot2017;
    
    //awaiting a real strategy ...
    void Strategy_Alpha(Robot2017& robot);

    //testing purpose
    void Strategy_Tanguy(Robot2017& robot);

    //for debug only : make all leds blinking to check they are working
    void Strategy_LedTest(Robot2017& robot);

    //for debug only : light a led for each activated user switch
    void Strategy_ButtonTest(Robot2017& robot);

    //for debug only : light a led for each activated omron
    void Strategy_OmronTest(Robot2017& robot);

    //for debug only : simple moves to check odometry
    void Strategy_CalibTest(Robot2017& robot);

    //for debug only : test all possible moves
    void Strategy_MotionTest(Robot2017& robot);
}

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_STRATEGIES_H_ */

