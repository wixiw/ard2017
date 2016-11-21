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
    ardAssert(NULL != io, "fio_manager_registerIn received a null pointer");
    ardAssert(fio_manager_nbIo < FIO_MANAGER_MAX_IO, "fio_manager is full, can't add a new pin");
    fio_manager_table[fio_manager_nbIo++] = io;
}

void ard::gpioToolsIsrCallback(uint32_t period_us)
{
    for( int i = 0 ; i < fio_manager_nbIo ; i++)
    {
        fio_manager_table[i]->update(period_us);
    }
}

FilteredInput::FilteredInput(uint8_t pinId,
        uint32_t debounceHigh,
        uint32_t debounceLow)
    : pin(pinId)
    , rising_CB(0)
    , falling_CB(0)
{
    reset();
    fio_manager_registerIn(this);
    setDebounceHigh(debounceHigh);
    setDebounceLow(debounceLow);
}

void FilteredInput::setDebounceHigh(uint32_t debounce)
{
    debounceHighDuration = debounce*1E3; //millis to us
}

void FilteredInput::setDebounceLow(uint32_t debounce)
{
    debounceLowDuration = debounce*1E3; //millis to us
}

void FilteredInput::registerRisingEdge_CB(FilteredInput_CB callback)
{
    rising_CB = callback;
}

void FilteredInput::registerFallingEdge_CB(FilteredInput_CB callback)
{
    falling_CB = callback;
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
    if( digitalRead(pin) == GPIO_HIGH && filteredLevel == GPIO_LOW)
    {
        debounceHighCount += period_us;

        if( debounceHighDuration <= debounceHighCount )
        {
            debounceLowCount = 0;
            filteredLevel = GPIO_HIGH;
            if( rising_CB != 0 )
                rising_CB();
        }
    }
    else if( digitalRead(pin) == GPIO_LOW && filteredLevel == GPIO_HIGH )
    {
        debounceLowCount += period_us;

        if( debounceLowDuration <= debounceLowCount )
        {
            debounceHighCount = 0;
            filteredLevel = GPIO_LOW;
            if( falling_CB != 0 )
                falling_CB();
        }
    }
    else
    {
        debounceLowCount = 0;
        debounceHighCount = 0;
    }
}

