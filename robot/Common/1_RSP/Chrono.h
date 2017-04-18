#ifndef CHRONO_H
#define CHRONO_H

#include "BSP.hpp"
#include "CommonMsg.pb.h"

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
    class Chrono
    {
    public:
        Chrono();

        // to be called ONCE when the match begins
        void startMatch();

        // Return the chrono since the beginning of the match
        uint32_t getChrono_ms();

        // Return how much time is left before the end of the match
        // Return match duration if match is not started
        uint32_t getTimeLeftMatch_ms();

        // Return how much time is left before the end of the funny action
        // Return funny action duration if the funny action is not started
        uint32_t getTimeLeftFunnyAction_ms();

        bool isMatchStarted(){ return matchStarted; }
        bool isFunnyActionStarted(){ return funnyActionStarted; }

        apb_Chrono getChrono();

        //TODO(QPE)
        //Reread the configuration and maps default config. Shall be called at least once
        //before the OS is initialized
//        void updateConf(RobotConfig* newConf);

    private:
        // The date in ms when the match starts
        uint32_t startDate_ms;

        bool matchStarted;
        bool funnyActionStarted;

        // Duration of the match (without funnyAction)
        uint32_t matchDuration_ms;

        // Duration of the funnyAction
        uint32_t funnyActionDuration_ms;
    };
}    //end namespace

#endif
