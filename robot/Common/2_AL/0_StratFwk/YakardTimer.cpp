/*
 * YakardTimer.cpp
 *
 *  Created on: 17 avr. 2017
 *      Author: wix
 */

#include "YakardTimer.h"
#ifdef BUILD_STRATEGY

using namespace ard;

YakardTimer::YakardTimer()
{
    INIT_TABLE_TO_ZERO(timers);
}

void YakardTimer::setTimer(TimedStatemachineInterface* statemachine, sc_eventid event, sc_integer time, sc_boolean isPeriodic)
{
    bool found = false;

    /* go through all timers ... */
    for (int i = 0; i < NB_YAKARD_TIMERS; i++)
    {

        /* ... and find an unused one. */
        if (timers[i].pt_evid == 0)
        {

            /* set timer properties */
            timers[i].pt_evid = event;
            timers[i].time_ms = time;
            timers[i].periodic = isPeriodic;
            timers[i].statemachine = statemachine;

            // reset the elapsed time ...
            timers[i].elapsed_time_ms = 0;
            found = true;
            break;
        }
    }

    ASSERT_TEXT(found,"YakardTimer timer list is too little, increase NB_YAKARD_TIMERS");
}

void YakardTimer::unsetTimer(TimedStatemachineInterface* statemachine, sc_eventid event)
{
    int i;

    for (i = 0; i < NB_YAKARD_TIMERS; i++)
    {
        if (timers[i].pt_evid == event)
        {

            timers[i].pt_evid = 0;
            timers[i].statemachine = NULL;

            break;
        }
    }
}

void YakardTimer::cancel()
{

}

void YakardTimer::run(DelayMs time_ms)
{
    /* go through all timers ... */
    for (int i = 0; i < NB_YAKARD_TIMERS; i++)
    {
        /* ... and process all used. */
        if (timers[i].pt_evid != 0)
        {

            if (timers[i].elapsed_time_ms < timers[i].time_ms)
            {
                timers[i].elapsed_time_ms += time_ms;

                if (timers[i].elapsed_time_ms >= timers[i].time_ms)
                {
                    //activates timer
                    timers[i].statemachine->raiseTimeEvent(timers[i].pt_evid);

                    if (timers[i].periodic)
                    {
                        timers[i].elapsed_time_ms -= timers[i].time_ms;
                    }
                }
            }
        }
    }
}

#endif
