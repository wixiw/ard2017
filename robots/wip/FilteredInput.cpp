/*
 * FilteredGpio.cpp
 *
 *  Created on: Apr 17, 2016
 *      Author: willy
 */

#include "os/error_hook.h"
#include "FilteredInput.hpp"

#define FIO_MANAGER_MAX_IO 20
FilteredInput* fio_manager_table[FIO_MANAGER_MAX_IO];
uint8_t fio_manager_nbIo = 0;

void fio_manager_registerIn(FilteredInput* io)
{
    ardAssert(0 != io);
    ardAssert(fio_manager_nbIo < FIO_MANAGER_MAX_IO);
    fio_manager_table[fio_manager_nbIo++] = io;
}

long ioManagerIsrCallback(uint32_t period_us)
{
    for( int i = 0 ; i < fio_manager_nbIo ; i++)
    {
        fio_manager_table[i]->update(period_us);
    }
    return 0;
}

FilteredInput::FilteredInput(GpioIn& _pin,
        uint32_t debounceHigh,
        uint32_t debounceLow)
    : pin(_pin)
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
    if( pin.read() == GPIO_HIGH && filteredLevel == GPIO_LOW)
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
    else if( pin.read() == GPIO_LOW && filteredLevel == GPIO_HIGH )
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

