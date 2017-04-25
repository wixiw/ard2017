/*
 * WipTration.h
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_WipTration_WipTration_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_WipTration_WipTration_H_

#include "0_Strategy2017.h"

#ifdef BUILD_STRATEGY
#include "generated/FSM_WipTration.h"

namespace ard
{
    class Robot2017;

    class WipTration: public Strategy2017
    {
    public:
        WipTration(Robot2017* robot);

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
        FSM_WipTration fsm;
    };

} /* namespace ard */

#endif //BUILD_STRATEGY

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_WipTration_WipTration_H_ */
