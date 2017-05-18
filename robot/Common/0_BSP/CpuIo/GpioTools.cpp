#include "../CpuIo/GpioTools.h"

#include "ArdOs.h"

using namespace ard;

/**
 * Configure the maximal number of gpio to check
 */
#define FIO_MANAGER_MAX_IO 25

PolledObject* fio_manager_table[FIO_MANAGER_MAX_IO];
uint8_t fio_manager_nbIo = 0;

void fio_manager_registerIn(PolledObject* io)
{
    ASSERT_TEXT(NULL != io, "fio_manager_registerIn received a null pointer");
    ASSERT_TEXT(fio_manager_nbIo < FIO_MANAGER_MAX_IO, "fio_manager is full, can't add a new pin");
    fio_manager_table[fio_manager_nbIo++] = io;
}

void ard::gpioToolsIsrCallback(DelayUs period_us)
{
    ASSERT(period_us);
    for (int i = 0; i < fio_manager_nbIo; i++)
    {
        ASSERT(fio_manager_table[i]);
        fio_manager_table[i]->update(period_us);
    }
}

FilteredInput::FilteredInput(uint8_t pinId, DelayMs debounceHigh, DelayMs debounceLow, bool invert):
        pin(pinId),
        invert(invert)
{
    reset();
    fio_manager_registerIn(this);
    setDebounceHigh(debounceHigh);
    setDebounceLow(debounceLow);
}

void FilteredInput::setDebounceHigh(DelayMs debounce)
{
    debounceHighDuration = debounce * 1E3; //millis to us
}

void FilteredInput::setDebounceLow(DelayMs debounce)
{
    debounceLowDuration = debounce * 1E3; //millis to us
}

void FilteredInput::reset()
{
    debounceHighCount = 0;
    debounceLowCount = 0;
    if(invert)
        filteredLevel = GPIO_HIGH;
    else
        filteredLevel = GPIO_LOW;
}

void FilteredInput::update(DelayUs sinceLastCall)
{
    int newPinState;

    if(invert)
        newPinState = 1 - digitalRead(pin);
    else
        newPinState = digitalRead(pin);


    //Pin rising edge
    if (newPinState == GPIO_HIGH && filteredLevel == GPIO_LOW)
    {
        debounceHighCount += sinceLastCall;

        if (debounceHighDuration <= debounceHighCount)
        {
            debounceLowCount = 0;
            filteredLevel = GPIO_HIGH;
        }
    }
    else if (newPinState == GPIO_LOW && filteredLevel == GPIO_HIGH)
    {
        debounceLowCount += sinceLastCall;

        if (debounceLowDuration <= debounceLowCount)
        {
            debounceHighCount = 0;
            filteredLevel = GPIO_LOW;
        }
    }
    else
    {
        debounceLowCount = 0;
        debounceHighCount = 0;
    }
}

PwmOutput::PwmOutput(uint8_t pinId, DelayUs period, bool invert):
        pin(pinId),
        period(period),
        transitionTime(0),
        cycleTime(0),
        invert(invert)
{
    fio_manager_registerIn(this);
}

eGpioLevel PwmOutput::read()
{
    if(invert)
        return (1 - digitalRead(pin)) ? GPIO_HIGH : GPIO_LOW;
    else
        return digitalRead(pin) ? GPIO_HIGH : GPIO_LOW;
}

void PwmOutput::update(DelayUs sinceLastCall)
{
    //case 100%
    if(transitionTime == period)
    {
        if(invert)
            digitalWrite(pin, 0);
        else
            digitalWrite(pin, 1);

        return;
    }

    //case 0%
    if(transitionTime == 0)
    {
        if(invert)
            digitalWrite(pin, 1);
        else
            digitalWrite(pin, 0);

        return;
    }

    uint16_t prev = cycleTime;
    cycleTime += sinceLastCall;
    ASSERT(prev < cycleTime);//overflow detection

    //First part of PWM : high level
    if( cycleTime < transitionTime)
    {
        if(invert)
            digitalWrite(pin, 0);
        else
            digitalWrite(pin, 1);

        return;
    }

    //Second part of PWM : low signal (except if PWM is 100)
    if(invert)
        digitalWrite(pin, 1);
    else
        digitalWrite(pin, 0);

    //Change cycle
    if(period <= cycleTime)
    {
        cycleTime = 0;
    }
}
