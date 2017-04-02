#ifndef GPIOTOOLS_HPP_
#define GPIOTOOLS_HPP_

#include <Arduino.h>
#include "ArdOs.h"

#define ARD_SET_BIT(field, bit, value) (field) ^= (-(value) ^ (field)) & (1 << (bit))
#define ARD_IS_BIT_SET(field, bit) (((field) >> (bit)) & 1)
#define ARD_BIT_TOGGLE(field, bit) (field) ^= 1 << (bit)z
namespace ard
{

    typedef enum
    {
        GPIO_LOW = 0, GPIO_HIGH = 1,
    } eGpioLevel;

    typedef enum
    {
        FALLING_EDGE = 0x01, RISING_EDGE = 0x02, ANY_EDGE = 0x03,
    } eGpioEdge;

    /**
     * This function has to be registered somewhere in a thread or an IT
     * Note that it calls FreeRtos calls, so it shall be configured at a
     * priority allowed but ISR-safe API of FreeRtos
     */
    void
    gpioToolsIsrCallback(DelayUs period_us);

    /**
     * This class is used to introduce filters on an input gpio.
     */
    class FilteredInput : public PolledObject
    {
    public:
        static const bool INVERTED = true;

        /**
         * @param pin : the gpio on which the filter is attached
         * @param debounceHigh : see setDebounceHigh
         * @param debounceLow : see setDebounceLow
         * @param invert : invert the signal level. The signal is inverted at the far beginning
         *                  of the computation as if the electronic signal were inverted
         *                  hence any reference to rising/falling edge events is done
         *                  on the inverted signal.
         */
        FilteredInput(uint8_t pinId, uint32_t debounceHigh = 0, uint32_t debounceLow = 0, bool invert = false);

        /**
         *  Add a debounce filter on the gpio for rising level
         *  Note : the debounce applies on the inverted raw signal
         *  @param debounce : the minimal duration of a high
         *        level change to consider it (in milliseconds)
         *        set to 0 to deactivate
         */
        void
        setDebounceHigh(uint32_t debounce);

        /**
         *  Add a debounce filter on the gpio for lowering level
         *  Note : the debounce applies on the inverted raw signal
         *  @param debounce : the minimal duration of a high
         *        level change to consider it (in milliseconds)
         *        set to 0 to deactivate
         */
        void
        setDebounceLow(uint32_t debounce);

        /**
         * Get the event reference to be able to wait for it
         */
        IEvent*
        getEvent(eGpioEdge edge);

        /**
         * Reset the filter
         * Note : the ouput filter value is set to HIGH when the signal is inverted.
         */
        void
        reset();

        /**
         * Read the filtered value (note the the inversion in taken into account)
         */
        eGpioLevel read() const
        {
            return filteredLevel;
        }

        /**
         * Read the raw inverted value (unfiltered)
         */
        eGpioLevel readRaw() const
        {
            if(invert)
                return digitalRead(pin) ? GPIO_LOW : GPIO_HIGH;
            else
                return digitalRead(pin) ? GPIO_HIGH : GPIO_LOW;
        }

        /**
         * Returns true if the raw inverted input is high, and the filtered output is low
         */
        bool isRising() const
        {
            return readRaw() == GPIO_HIGH && filteredLevel == GPIO_LOW;
        }

        /**
         * Returns true if the raw inverted input is low, and the filtered output is high
         */
        bool isFalling() const
        {
            return readRaw() == GPIO_LOW && filteredLevel == GPIO_HIGH;
        }

        /**
         * reserved for FilteredInputManager, do not call
         */
        void update(DelayMs sinceLastCall) override;

    private:
        uint8_t pin;
        bool invert;
        eGpioLevel filteredLevel;
        DelayUs debounceHighDuration;
        volatile uint32_t debounceHighCount;
        DelayUs debounceLowDuration;
        volatile uint32_t debounceLowCount;
        Event<1> eventAny;
        Event<1> eventFalling;
        Event<1> eventRising;
    };

    /**
     * This class is used to create a pwm on a normal gpio
     * Typical use for leds
     */
    class PwmOutput : public PolledObject
    {
    public:
        static const bool INVERTED = true;

        /**
         * @param pin : the gpio on which the filter is attached
         * @param period : duration of the PWM signal in ms
         * @param invert : invert the signal level.
         */
        PwmOutput(uint8_t pinId, DelayUs period, bool invert = false);

        /**
         * @param per1000 : 1000th of high signal (0%° always 0, 1000%° always 1)
         */
        void setPWM(uint16_t per1000)
        {
            if( 1000 < per1000 )
                ASSERT(false);
            else
                transitionTime = (per1000*period)/1000;
        };

        /**
         * reserved for FilteredInputManager, do not call
         */
        void update(DelayUs sinceLastCall) override;

    private:
        uint8_t pin;
        DelayUs period;
        DelayUs transitionTime; //the time in a cycle at which the signal is changed to 0
        DelayUs cycleTime; //counts the elasped time in the cycle
        bool invert;

    };

} //end namespace

#endif /* GPIOTOOLS_HPP_ */
