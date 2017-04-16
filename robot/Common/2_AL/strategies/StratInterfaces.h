/*
 * StratInterfaces.h
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_STRATINTERFACES_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_STRATINTERFACES_H_

#include <stdint.h>
#include "RSP.h"

namespace ard
{
    //forward declarations
    class Robot2017;


    typedef enum
    {
        LSA_Success,
        LSA_Failed,
    } LSA_result;

    /**
     * LSA stands for : local strategic action. An LSA is a subpart of a global strategy.
     * The idea is to separate strategy concerns into :
     * - simple actions with few moves and specific avoidance and mess management
     * - global map displacement to go from one action to another with a generic avoidance system
     * Moreover such a design allows to delegate to an IA the order in which actions are executed.
     *
     * You have to inheritate this interface and to implement the action in the execute method
     */
    class LSA
    {
    public:
        LSA(Robot2017& robot, PointCap const& entryPoint):
            robot(robot),
            entryPoint(entryPoint){};

        virtual ~LSA() = default;

        //Get the expected position of the robot when the LSA begins
        virtual PointCap const& getEntryPoint() const { return entryPoint;};

        //Execute the LSA for nb items, 0 would mean maximum number
        virtual LSA_result execute(uint8_t nbItems = 0) = 0;

    protected:
        Robot2017& robot;
        PointCap entryPoint;
    };
}

   //exemple :
//    class LSA_Dispenser_Opp: public LSA
//    {
//    public:
//        LSA_Dispenser_Opp(Robot2017& robot);
//
//        //Implements LSA
//        virtual LSA_result execute(uint8_t nbItems = 0) override;
//    };
//}

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_STRATINTERFACES_H_ */
