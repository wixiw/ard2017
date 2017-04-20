/*
 * Willy.h
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_WILLY_WILLY_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_WILLY_WILLY_H_

#include "ArdOs.h"
#include "StratFwk.h"

#ifdef BUILD_STRATEGY
#include "generated/FSM_Willy.h"

namespace ard
{

    class Willy: public Strategy2017
    {
    public:
        Willy(TimerInterface& timer);

        /**---------------------------------
         * Container thread interface
         ---------------------------------*/

        //Implements IMiniThread: init the state machine
        void init() override;

        //Implements IMiniThread : method to be called by the container thread
        //                         it's expected to be called periodically
        void update(TimeMs sinceLastCall) override;

    private:
        FSM_Willy fsm;
    };

} /* namespace ard */

#endif //BUILD_STRATEGY

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_WILLY_WILLY_H_ */
