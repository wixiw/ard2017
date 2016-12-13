#include "GpioTools.h"
#include "ArdOs.h"

using namespace ard;

/**
 * Configure the maximal number of gpio to check
 */
#define FIO_MANAGER_MAX_IO 10

FilteredInput* fio_manager_table[FIO_MANAGER_MAX_IO];
uint8_t fio_manager_nbIo = 0;

void
fio_manager_registerIn (FilteredInput* io)
{
  ardAssert(NULL != io, "fio_manager_registerIn received a null pointer");
  ardAssert(fio_manager_nbIo < FIO_MANAGER_MAX_IO,
	    "fio_manager is full, can't add a new pin");
  fio_manager_table[fio_manager_nbIo++] = io;
}

void
ard::gpioToolsIsrCallback (uint32_t period_us)
{
  for (int i = 0; i < fio_manager_nbIo; i++)
    {
      fio_manager_table[i]->update (period_us);
    }
}

void
ard::gpioInit ()
{
  for (int i = 0; i < fio_manager_nbIo; i++)
    {
      fio_manager_table[i]->init ();
    }
}

FilteredInput::FilteredInput (uint8_t pinId, uint32_t debounceHigh,
			      uint32_t debounceLow) :
    pin (pinId), signalAny (NULL), signalFalling (NULL), signalRising (NULL)
{
  reset ();
  fio_manager_registerIn (this);
  setDebounceHigh (debounceHigh);
  setDebounceLow (debounceLow);
}

void
FilteredInput::setDebounceHigh (uint32_t debounce)
{
  debounceHighDuration = debounce * 1E3; //millis to us
}

void
FilteredInput::setDebounceLow (uint32_t debounce)
{
  debounceLowDuration = debounce * 1E3; //millis to us
}

void
FilteredInput::wait (eGpioEdge edge)
{
  switch (edge)
    {
    default:
    case ANY_EDGE:
      g_ArdOs.Signal_wait(signalAny);
      break;

      case FALLING_EDGE:
      g_ArdOs.Signal_wait(signalFalling);
      break;

      case RISING_EDGE:
      g_ArdOs.Signal_wait(signalRising);
      break;
    }
}

void
FilteredInput::reset ()
{
  debounceHighCount = 0;
  debounceLowCount = 0;
  filteredLevel = GPIO_LOW;
}

void
FilteredInput::update (uint32_t period_us)
{
  //Pin rising edge
  if (digitalRead (pin) == GPIO_HIGH && filteredLevel == GPIO_LOW)
    {
      debounceHighCount += period_us;

      if (debounceHighDuration <= debounceHighCount)
	{
	  debounceLowCount = 0;
	  filteredLevel = GPIO_HIGH;
	  g_ArdOs.Signal_setFromIsr(signalAny);
	  g_ArdOs.Signal_setFromIsr(signalRising);
	}
    }
  else if( digitalRead(pin) == GPIO_LOW && filteredLevel == GPIO_HIGH )
    {
      debounceLowCount += period_us;

      if( debounceLowDuration <= debounceLowCount )
	{
	  debounceHighCount = 0;
	  filteredLevel = GPIO_LOW;
	  g_ArdOs.Signal_setFromIsr(signalAny);
	  g_ArdOs.Signal_setFromIsr(signalFalling);
	}
    }
  else
    {
      debounceLowCount = 0;
      debounceHighCount = 0;
    }
}

void
FilteredInput::init ()
{
  signalAny = g_ArdOs.Signal_create();
  signalFalling = g_ArdOs.Signal_create();
  signalRising = g_ArdOs.Signal_create();
}

void FilteredInput::fakeEdge(eGpioEdge edge)
{
    if(edge == RISING_EDGE)
    {
        g_ArdOs.Signal_set(signalAny);
        g_ArdOs.Signal_set(signalRising);
        return;
    }
    if(edge == FALLING_EDGE)
    {
        g_ArdOs.Signal_set(signalAny);
        g_ArdOs.Signal_set(signalFalling);
        return;
    }
    ardAssert(false, "Unexpected edge in FilteredInput::fakeEdge.");
}

