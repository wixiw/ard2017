/*
 * LSAitf.h
 *
 *  Created on: 25 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_2_LSA_LSA2017_H_
#define ROBOT_COMMON_2_AL_2_LSA_LSA2017_H_

#include "Action2017.h"
#include "RSP.h"

namespace ard
{
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
        void stopLSA(){Action2017<FSM, States_t>::stop();}
        StrategyResult getStatusLSA(){return Action2017<FSM, States_t>::getStatus();}

    protected:
        PointCap entryPoint;
    };
} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_2_LSA_LSA2017_H_ */
