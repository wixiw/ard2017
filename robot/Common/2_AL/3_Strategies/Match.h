/*
 * Match.h
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_Match_Match_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_Match_Match_H_

#include "ArdOs.h"
#include "StratFwk.h"

#ifdef BUILD_STRATEGY
#include "generated/FSM_Match.h"

namespace ard
{

    class Match: public Strategy2017
    {
    public:
        Match(TimerInterface& timer);

        /**---------------------------------
         * Container thread interface
         ---------------------------------*/

        //Implements IMiniThread: init the state machine
        void init() override;

        //Implements IMiniThread : method to be called by the container thread
        //                         it's expected to be called periodically
        void update(TimeMs sinceLastCall) override;

    private:
        FSM_Match fsm;
    };

} /* namespace ard */

#endif //BUILD_STRATEGY

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_Match_Match_H_ */
