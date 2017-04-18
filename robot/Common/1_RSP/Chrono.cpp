#include <Arduino.h>
#include "K_constants.h"
#include "Log.h"
#include "Chrono.h"

using namespace ard;

Chrono::Chrono()
        :
        startDate_ms(UINT32_MAX),
        matchStarted(false),
        funnyActionStarted(false),
        matchDuration_ms(90000),
        funnyActionDuration_ms(5000)
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

uint32_t
Chrono::getChrono_ms()
{
    if (matchStarted)
        return millis() - startDate_ms;
    else
        return 0;
}

uint32_t
Chrono::getTimeLeftMatch_ms()
{
    if (matchStarted)
    {
        uint32_t date = millis();
        if (matchDuration_ms > (date - startDate_ms))
            return (matchDuration_ms - (date - startDate_ms));
        else
            return 0;
    }
    else
        return matchDuration_ms;
}

apb_Chrono
Chrono::getChrono()
{
    apb_Chrono chrono;
    chrono.started = matchStarted;
    chrono.chrono_ms = getChrono_ms();
    chrono.timeLeft_ms = getTimeLeftMatch_ms();
    return chrono;
}


