/*
 * Tanguy.h
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_Tanguy_Tanguy_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_Tanguy_Tanguy_H_

#include "ArdOs.h"
#include "StratFwk.h"

#ifdef BUILD_STRATEGY
#include "generated/FSM_Tanguy.h"

namespace ard
{

    class Tanguy: public Strategy2017
    {
    public:
        Tanguy(TimerInterface& timer);

        /**---------------------------------
         * Container thread interface
         ---------------------------------*/

        //Implements IMiniThread: init the state machine
        void init() override;

        //Implements IMiniThread : method to be called by the container thread
        //                         it's expected to be called periodically
        void update(TimeMs sinceLastCall) override;

    private:
        FSM_Tanguy fsm;
    };

} /* namespace ard */

#endif //BUILD_STRATEGY

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_Tanguy_Tanguy_H_ */
