/*
 * FilteredInput.hpp
 *
 *  Created on: Apr 17, 2016
 *      Author: willy
 */

#ifndef FILTEREDINPUT_HPP_
#define FILTEREDINPUT_HPP_

#include "core/cpuio/Gpio.hpp"

/**
 * This function has to be registered somewhere in a thread or an IT
 */
long ioManagerIsrCallback(uint32_t period_us);

/**
 * This class is used to introduce filters on an input gpio.
 * You may instanciate several FilteredGpio for a unique io
 * without any problem
 */
class FilteredInput : public IGpioIn
{
    friend long ioManagerIsrCallback(uint32_t period_us);

    public:
        typedef void (*FilteredInput_CB)(void);

        /**
         * @param pin : the gpio on which the filter is attached
         * @param debounceHigh : see setDebounceHigh
         * @param debounceLow : see setDebounceLow
         */
        FilteredInput(GpioIn& _pin, uint32_t debounceHigh = 0, uint32_t debounceLow = 0);

        /**
         *  Add a debounce filter on the gpio for rising level
         *  @param debounce : the minimal duration of a high
         *        level change to consider it (in milliseconds)
         *        set to 0 to deactivate
         */
        void setDebounceHigh(uint32_t debounce);

        /**
         *  Add a debounce filter on the gpio for lowering level
         *  @param debounce : the minimal duration of a high
         *        level change to consider it (in milliseconds)
         *        set to 0 to deactivate
         */
        void setDebounceLow(uint32_t debounce);

        /**
         * Register a function that will be called
         * when the filtered output goes from low to high
         * a NULL value mean : cancel the current Callback
         */
        void registerRisingEdge_CB(FilteredInput_CB callback);

        /**
         * Register a function that will be called
         * when the filtered output goes from high to low
         * a NULL value mean : cancel the current Callback
         */
        void registerFallingEdge_CB(FilteredInput_CB callback);

        /**
         * Reset the filter
         */
        void reset();

        /**
         * Read the filtered value
         */
        GpioLevel read() const
        {
            return filteredLevel;
        }

        /**
         * Read the raw value (unfiltered
         */
        GpioLevel readRaw() const
        {
            return pin.read();
        }

        /**
         * Returns true if the raw input is high, and the filtered output is low
         */
        bool isRising() const
        {
            return pin.read() == GPIO_HIGH && filteredLevel == GPIO_LOW;
        }

        /**
         * Returns true if the raw input is low, and the filtered output is high
         */
        bool isFalling() const
        {
            return pin.read() == GPIO_LOW && filteredLevel == GPIO_HIGH;
        }

    protected:
        /**
         * reserved for FilteredInputManager
         */
        void update(uint32_t period_us);

    private:
        GpioIn& pin;
        GpioLevel filteredLevel;
        uint32_t debounceHighDuration;
        volatile uint32_t debounceHighCount;
        uint32_t debounceLowDuration;
        volatile uint32_t debounceLowCount;
        FilteredInput_CB rising_CB;
        FilteredInput_CB falling_CB;
};

#endif /* FILTEREDINPUT_HPP_ */
