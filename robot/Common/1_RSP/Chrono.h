#ifndef CHRONO_H
#define CHRONO_H

#include "BSP.hpp"
#include "CommonMsg.pb.h"
#include "RobotParameters.h"

namespace ard
{
    /**
     * This class manage the time during the match,
     * once the match is started
     *
     * CONVENTION :
     *
     *  preMatch              Match               FunnyAction   postMatch
     * ----------+------------------------------+-------------+-------------
     *           ^                              ^             ^
     *           START PULLED                   T+90s         T+95s
     */
    class Chrono : public RobotParametersListener
    {
    public:
        Chrono();

        // to be called ONCE when the match begins
        void startMatch();

        //stop the chrono when match is finished (including funny action)
        void stop();

        // Return the chrono since the beginning of the match
        TimeMs getTime() const;

        // Return how much time is left before the end of the match
        // Return match duration if match is not started
        DelayMs getStrategyRemainingTime() const;

        // Return how much time is left before the end of the funny action
        // Return funny action duration if the funny action is not started
        //uint32_t getTimeLeftFunnyAction_ms();

        //provide telemetry data
        apb_Chrono serialize() const;

    private:
        // The date in ms when the match starts
        TimeMs startDate_ms;

        bool matchStarted;
    };
}    //end namespace

#endif
