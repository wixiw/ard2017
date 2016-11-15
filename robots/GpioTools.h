#ifndef GPIOTOOLS_HPP_
#define GPIOTOOLS_HPP_

#include <Arduino.h>

#define ARD_SET_BIT(field, bit, value) (field) ^= (-(value) ^ (field)) & (1 << (bit))
#define ARD_IS_BIT_SET(field, bit) (((field) >> (bit)) & 1)
#define ARD_BIT_TOGGLE(field, bit) (field) ^= 1 << (bit)

namespace ard
{

  typedef enum
  {
    GPIO_LOW = 0, GPIO_HIGH = 1,
  } eGpioLevel;

  typedef enum
  {
    FALLING_EDGE = 0x01, RISING_EDGE = 0x02, EVERY_EDGE = 0x03,
  } eGpioEdge;

  /**
   * This function has to be registered somewhere in a thread or an IT
   */
  long
  gpioToolsIsrCallback (uint32_t period_us);

  /**
   * This class is used to introduce filters on an input gpio.
   * You may instanciate several FilteredGpio for a unique io
   * without any problem
   */
  class FilteredInput
  {
  public:
    typedef void
    (*FilteredInput_CB) (void);

    /**
     * @param pin : the gpio on which the filter is attached
     * @param debounceHigh : see setDebounceHigh
     * @param debounceLow : see setDebounceLow
     */
    FilteredInput (uint8_t pinId, uint32_t debounceHigh = 0,
		   uint32_t debounceLow = 0);

    /**
     *  Add a debounce filter on the gpio for rising level
     *  @param debounce : the minimal duration of a high
     *        level change to consider it (in milliseconds)
     *        set to 0 to deactivate
     */
    void
    setDebounceHigh (uint32_t debounce);

    /**
     *  Add a debounce filter on the gpio for lowering level
     *  @param debounce : the minimal duration of a high
     *        level change to consider it (in milliseconds)
     *        set to 0 to deactivate
     */
    void
    setDebounceLow (uint32_t debounce);

    /**
     * Register a function that will be called
     * when the filtered output goes from low to high
     * a NULL value mean : cancel the current Callback
     */
    void
    registerRisingEdge_CB (FilteredInput_CB callback);

    /**
     * Register a function that will be called
     * when the filtered output goes from high to low
     * a NULL value mean : cancel the current Callback
     */
    void
    registerFallingEdge_CB (FilteredInput_CB callback);

    /**
     * Reset the filter
     */
    void
    reset ();

    /**
     * Read the filtered value
     */
    eGpioLevel
    read () const
    {
      return filteredLevel;
    }

    /**
     * Read the raw value (unfiltered
     */
    eGpioLevel
    readRaw () const
    {
      return digitalRead (pin) ? GPIO_HIGH : GPIO_LOW;
    }

    /**
     * Returns true if the raw input is high, and the filtered output is low
     */
    bool
    isRising () const
    {
      return digitalRead (pin) == GPIO_HIGH && filteredLevel == GPIO_LOW;
    }

    /**
     * Returns true if the raw input is low, and the filtered output is high
     */
    bool
    isFalling () const
    {
      return digitalRead (pin) == GPIO_LOW && filteredLevel == GPIO_HIGH;
    }

    /**
     * reserved for FilteredInputManager, do not call
     */
    void
    update (uint32_t period_us);

  private:
    uint8_t pin;
    eGpioLevel filteredLevel;
    uint32_t debounceHighDuration;
    volatile uint32_t debounceHighCount;
    uint32_t debounceLowDuration;
    volatile uint32_t debounceLowCount;
    FilteredInput_CB rising_CB;
    FilteredInput_CB falling_CB;
  };
} //end namespace

#endif /* GPIOTOOLS_HPP_ */
