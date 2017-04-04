/*
 * Strategies.h
 *
 *  Created on: 16 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_STRATEGIES_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_STRATEGIES_H_

namespace ard
{
    //awaiting a real strategy ...
    void Strategy_Alpha();

    //testing purpose
    void Strategy_Tanguy();

    //for debug only : make all leds blinking to check they are working
    void Strategy_LedTest();

    //for debug only : light a led for each activated user switch
    void Strategy_ButtonTest();

    //for debug only : light a led for each activated omron
    void Strategy_OmronTest();

    //for debug only : simple moves to check odometry
    void Strategy_CalibTest();

    //for debug only : test all possible moves
    void Strategy_MotionTest();
}

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_STRATEGIES_H_ */
