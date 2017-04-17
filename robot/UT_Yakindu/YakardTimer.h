/*
 * YakardTimer.h
 *
 *  Created on: 17 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_UT_YAKINDU_YAKARDTIMER_H_
#define ROBOT_UT_YAKINDU_YAKARDTIMER_H_

#include "TimerInterface.h"

#define NB_YAKARD_TIMERS 10

namespace ard
{
    class YakardTimer: public TimerInterface
    {
    public:
        YakardTimer();

        //Implements TimerInterface
        void setTimer(TimedStatemachineInterface* statemachine, sc_eventid event, sc_integer time, sc_boolean isPeriodic) override;

        //Implements TimerInterface
        void unsetTimer(TimedStatemachineInterface* statemachine, sc_eventid event) override;

        //Implements TimerInterface
        void cancel() override;

        //Call this function reasonably quickly to generates timer events
        //@param time since last call
        void run(sc_integer time_ms);

    protected:
        /* internal arguments of a timer pthread */
        typedef struct {
            sc_integer time_ms;
            sc_boolean periodic;
            sc_integer elapsed_time_ms;
            sc_eventid pt_evid;
            TimedStatemachineInterface* statemachine;
        } sc_timer_t;

        sc_timer_t timers[NB_YAKARD_TIMERS];
    };

} /* namespace ard */

#endif /* ROBOT_UT_YAKINDU_YAKARDTIMER_H_ */
