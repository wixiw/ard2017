/*
 * HomolPen.h
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_HomolPen_HomolPen_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_HomolPen_HomolPen_H_

#include "0_Strategy2017.h"

#ifdef BUILD_STRATEGY
#include "generated/FSM_HomolPen.h"

namespace ard
{
    class Robot2017;

    typedef enum
    {
        RESERVED,
        LSA1,
        NB_LSA
    } eHomolPenId;

    /**
     * This class will simply run LSA in a raw
     */
    class HomolPen: public Strategy2017
    {
    public:
        HomolPen(Robot2017* robot);

        /**---------------------------------
         * Container thread interface
         ---------------------------------*/

        //Implements IMiniThread: init the state machine
        void init() override;

        //Implements IMiniThread : method to be called by the container thread
        //                         it's expected to be called periodically
        void update(TimeMs sinceLastCall) override;

        STRAT_2017_API_ITF();

    private:
        FSM_HomolPen fsm;

    };

} /* namespace ard */

#endif //BUILD_STRATEGY

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_HomolPen_HomolPen_H_ */
