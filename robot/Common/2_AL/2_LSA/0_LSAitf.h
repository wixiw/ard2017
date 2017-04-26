/*
 * LSAitf.h
 *
 *  Created on: 25 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_2_LSA_0_LSAITF_H_
#define ROBOT_COMMON_2_AL_2_LSA_0_LSAITF_H_

#include "RSP.h"
#ifdef BUILD_STRATEGY

#include "StratFwk.h"

namespace ard
{

    typedef enum
    {
        NoLsa,
        Success,
        Failed,
        InProgress,
    } LSAResult;

    /**
     * LSA stands for : local strategic action. An LSA is a subpart of a global strategy.
     * The idea is to separate strategy concerns into :
     * - simple actions with few moves and specific avoidance and mess management
     * - global map displacement to go from one action to another with a generic avoidance system
     * Moreover such a design allows to delegate to an IA the order in which actions are executed.
     *
     * You have to inheritate this interface and to implement the action in the execute method
     */
    class LSA: public Action2017
    {
    public:
        LSA(Robot2017* robot, String const& name, PointCap const& entryPoint = PointCap()):
            Action2017(robot, name),
            entryPoint(entryPoint){};

        virtual ~LSA() = default;

        //Get the expected position of the robot when the LSA begins
        PointCap const& getEntryPoint() const { return entryPoint;};
        void setEntryPoint(PointCap const& point){entryPoint=point;};

        //Reset the machine and starts
        virtual void start() = 0;

        //Get information about machine status
        virtual LSAResult isFinished() = 0;

    protected:
        PointCap entryPoint;
    };
} /* namespace ard */

#endif
#endif /* ROBOT_COMMON_2_AL_2_LSA_0_LSAITF_H_ */
