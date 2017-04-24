#include <Arduino.h>
#include "K_constants.h"
#include "Log.h"
#include "Chrono.h"

using namespace ard;

Chrono::Chrono()
        :
        startDate_ms(UINT32_MAX),
        matchStarted(false),
        strategyDuration_ms(90000)//,
//        funnyActionDuration_ms(5000)
{
}

void
Chrono::startMatch()
{
    // this can only be called once
    ASSERT_TEXT(!matchStarted, "match already started !");
    startDate_ms = millis();
    matchStarted = true;
}

TimeMs
Chrono::getTime() const
{
    if (matchStarted)
        return millis() - startDate_ms;
    else
        return 0;
}

TimeMs
Chrono::getStrategyRemainingTime() const
{
    if (matchStarted)
    {
        uint32_t date = millis();
        if (strategyDuration_ms > (date - startDate_ms))
            return (strategyDuration_ms - (date - startDate_ms));
        else
            return 0;
    }
    else
        return strategyDuration_ms;
}

apb_Chrono
Chrono::serialize() const
{
    apb_Chrono chrono;
    chrono.started = matchStarted;
    chrono.chrono_ms = getTime();
    chrono.timeLeft_ms = getStrategyRemainingTime();
    return chrono;
}

void Chrono::updateConf(RobotParameters* newConf)
{
    ASSERT(newConf);
    strategyDuration_ms = newConf->strategyDuration();
}
