#include <Arduino.h>
#include "Log.h"
#include "Chrono.h"

using namespace ard;

Chrono::Chrono():
        startDate_ms(UINT32_MAX),
        matchStarted(false)
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

void
Chrono::stop()
{
    ASSERT_TEXT(matchStarted, "match is not started !");
    matchStarted = false;
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
	ASSERT_CONFIGURED();
    if (matchStarted)
    {
        uint32_t date = millis();
        if (conf->strategyDuration() > (date - startDate_ms))
            return (conf->strategyDuration() - (date - startDate_ms));
        else
            return 0;
    }
    else
        return conf->strategyDuration();
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