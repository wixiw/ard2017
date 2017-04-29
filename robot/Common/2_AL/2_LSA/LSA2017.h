/*
 * LSAitf.h
 *
 *  Created on: 25 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_2_LSA_LSA2017_H_
#define ROBOT_COMMON_2_AL_2_LSA_LSA2017_H_

#include "RSP.h"
#include "0_Action2017.h"

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
     * This interface is used to be able to get a pointer on the LSA
     * without having to get the template meta data issues
     */
    class LSA
    {
    public:
        virtual ~LSA() = default;

        //Get the expected position of the robot when the LSA begins
        virtual PointCap const& getEntryPoint() const = 0;

        //Set the entry point
        virtual void setEntryPoint(PointCap const& point) = 0;

        //Get information about machine status
        virtual LSAResult isFinished() = 0;

        //design issue : users of LSA would like to poll it,
        //it's not possible to share the PolledObject interface
        //as it's also used in other class used in concrete LSA.
        //so to prevent loosing time as competition is approaching
        //this method replace the one of PolledObject and the child
        //class has to do the wrapping. Any idea welcome.
        virtual void updateLSA(DelayMs sinceLastCall) = 0;
        //Same remarks here
        virtual void startLSA() = 0;
    };

    /**
     * LSA stands for : local strategic action. An LSA is a subpart of a global strategy.
     * The idea is to separate strategy concerns into :
     * - simple actions with few moves and specific avoidance and mess management
     * - global map displacement to go from one action to another with a generic avoidance system
     * Moreover such a design allows to delegate to an IA the order in which actions are executed.
     *
     * You have to inheritate this interface and to implement the action in the execute method
     */
    template<class FSM, typename States_t>
    class LSA2017: public Action2017<FSM, States_t>, public LSA
    {
    public:
        LSA2017(Robot2017& robot, String const& name, PointCap const& entryPoint = PointCap()):
            Action2017<FSM, States_t>(robot, name),
            entryPoint(entryPoint){};

        virtual ~LSA2017() = default;

        //Implements LSA
        PointCap const& getEntryPoint() const override { return entryPoint;}

        //Implements LSA
        void setEntryPoint(PointCap const& point) override{entryPoint=point;}

        //Implements LSA
        void updateLSA(DelayMs sinceLastCall){Action2017<FSM, States_t>::update(sinceLastCall);}
        void startLSA(){Action2017<FSM, States_t>::start();}

    protected:
        PointCap entryPoint;
    };
} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_2_LSA_LSA2017_H_ */
