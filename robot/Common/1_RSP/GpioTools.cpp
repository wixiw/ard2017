#include "GpioTools.h"
#include "ArdOs.h"

using namespace ard;

/**
 * Configure the maximal number of gpio to check
 */
#define FIO_MANAGER_MAX_IO 10

FilteredInput* fio_manager_table[FIO_MANAGER_MAX_IO];
uint8_t fio_manager_nbIo = 0;

void fio_manager_registerIn(FilteredInput* io)
{
    ASSERT_TEXT(NULL != io, "fio_manager_registerIn received a null pointer");
    ASSERT_TEXT(fio_manager_nbIo < FIO_MANAGER_MAX_IO, "fio_manager is full, can't add a new pin");
    fio_manager_table[fio_manager_nbIo++] = io;
}

void ard::gpioToolsIsrCallback(uint32_t period_us)
{
    for (int i = 0; i < fio_manager_nbIo; i++)
    {
        fio_manager_table[i]->update(period_us);
    }
}

FilteredInput::FilteredInput(uint8_t pinId, uint32_t debounceHigh, uint32_t debounceLow)
        : pin(pinId)
{
    reset();
    fio_manager_registerIn(this);
    setDebounceHigh(debounceHigh);
    setDebounceLow(debounceLow);
}

void FilteredInput::setDebounceHigh(uint32_t debounce)
{
    debounceHighDuration = debounce * 1E3; //millis to us
}

void FilteredInput::setDebounceLow(uint32_t debounce)
{
    debounceLowDuration = debounce * 1E3; //millis to us
}

IEvent*
FilteredInput::getEvent(eGpioEdge edge)
{
    switch (edge)
    {
    default:
        ASSERT_TEXT(false, "FilteredInput::getEvent : unexpected value.");
        break;

    case ANY_EDGE:
        return &eventAny;
        break;

    case FALLING_EDGE:
        return &eventFalling;
        break;

    case RISING_EDGE:
        return &eventRising;
        break;
    }

    ASSERT_TEXT(false, "FilteredInput::getEvent : unreachable.");
    return NULL;
}

void FilteredInput::reset()
{
    debounceHighCount = 0;
    debounceLowCount = 0;
    filteredLevel = GPIO_LOW;
}

void FilteredInput::update(uint32_t period_us)
{
    //Pin rising edge
    if (digitalRead(pin) == GPIO_HIGH && filteredLevel == GPIO_LOW)
    {
        debounceHighCount += period_us;

        if (debounceHighDuration <= debounceHighCount)
        {
            debounceLowCount = 0;
            filteredLevel = GPIO_HIGH;
            eventAny.publishFromISR();
            eventRising.publishFromISR();
        }
    }
    else if (digitalRead(pin) == GPIO_LOW && filteredLevel == GPIO_HIGH)
    {
        debounceLowCount += period_us;

        if (debounceLowDuration <= debounceLowCount)
        {
            debounceHighCount = 0;
            filteredLevel = GPIO_LOW;
            eventAny.publishFromISR();
            eventFalling.publishFromISR();
        }
    }
    else
    {
        debounceLowCount = 0;
        debounceHighCount = 0;
    }
}
