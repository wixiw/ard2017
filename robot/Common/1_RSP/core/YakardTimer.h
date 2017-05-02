/*
 * YakardTimer.h
 *
 *  Created on: 17 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_UT_YAKINDU_YAKARDTIMER_H_
#define ROBOT_UT_YAKINDU_YAKARDTIMER_H_

#include "BSP.hpp"

#include "TimerInterface.h"
#include "TimedStatemachineInterface.h"

#define NB_YAKARD_TIMERS 10

namespace ard
{
    /**
     * Yakard stands for Yakindu-ARD. This class is an helper class multiplexing virtual SW timers so that Yakindu can work.
     * You have to register an instance of this class in any of your timed-state machine (a fsm is timed as soon as a each/after transition appears).
     * You have to trigger periodically this class
     */
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
        void run(DelayMs time_ms);

    protected:
        /* internal arguments of a timer pthread */
        typedef struct {
            DelayMs time_ms;
            bool periodic;
            DelayMs elapsed_time_ms;
            sc_eventid pt_evid;
            TimedStatemachineInterface* statemachine;
        } VirtualTimer;

        VirtualTimer timers[NB_YAKARD_TIMERS];
    };

} /* namespace ard */

#endif /* ROBOT_UT_YAKINDU_YAKARDTIMER_H_ */
