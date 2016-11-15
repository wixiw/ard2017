/*
 * GpioTools.cpp
 *
 *  Created on: 21 oct. 2016
 *      Author: wix
 */

#include "HmiThread.h"
#include "BSP.h"

using namespace ard;

/**
 * Configure the HMI buttons debounce time in ms.
 */
#define HMI_DEBOUNCE 100

/*--------------------------------------------------------------------- */
/*   OUTPUT                                                             */
/*--------------------------------------------------------------------- */

RgbLed::RgbLed (int pin_r, int pin_g, int pin_b)
{
  m_r = pin_r;
  m_g = pin_g;
  m_b = pin_b;

  m_color = eRgb::WHITE;
  m_blink = eLedState::OFF;

  lastState = 0;
  m_on = false;
}

void
RgbLed::set (eRgb color, eLedState blink)
{
  m_color = color;
  m_blink = blink;
}

void
RgbLed::off ()
{
  m_blink = eLedState::OFF;
}

void
RgbLed::run ()
{
  if ((lastState % 2 == 0 && m_blink == eLedState::FAST_BLINK)
  	  || (lastState % 10 == 0 && m_blink == eLedState::SLOW_BLINK))
    m_on = !m_on;
  else if (m_blink == eLedState::ON)
    m_on = true;
  else if (m_blink == eLedState::OFF)
    m_on = false;

  output (m_on);
  lastState++;
}

void
RgbLed::output (bool _on)
{
  if (_on)
    {
      switch (m_color)
	{
	case eRgb::RED:
	  digitalWrite (m_r, LOW);
	  digitalWrite (m_g, HIGH);
	  digitalWrite (m_b, HIGH);
	  break;
	case eRgb::GREEN:
	  digitalWrite (m_r, HIGH);
	  digitalWrite (m_g, LOW);
	  digitalWrite (m_b, HIGH);
	  break;
	case eRgb::BLUE:
	  digitalWrite (m_r, HIGH);
	  digitalWrite (m_g, HIGH);
	  digitalWrite (m_b, LOW);
	  break;
	case eRgb::YELLOW:
	  digitalWrite (m_r, LOW);
	  digitalWrite (m_g, LOW);
	  digitalWrite (m_b, HIGH);
	  break;
	case eRgb::CYAN:
	  digitalWrite (m_r, HIGH);
	  digitalWrite (m_g, LOW);
	  digitalWrite (m_b, LOW);
	  break;
	case eRgb::PURPLE:
	  digitalWrite (m_r, LOW);
	  digitalWrite (m_g, HIGH);
	  digitalWrite (m_b, LOW);
	  break;
	case eRgb::WHITE:
	  digitalWrite (m_r, LOW);
	  digitalWrite (m_g, LOW);
	  digitalWrite (m_b, LOW);
	  break;
	default:
	  ;
	}
    }
  else
    {
      digitalWrite (m_r, HIGH);
      digitalWrite (m_g, HIGH);
      digitalWrite (m_b, HIGH);
    }
}

//---------------------------------------------------------------------------

Led::Led (int pin, bool inverted) :
    m_blink (eLedState::OFF), m_pin (pin), m_inverted(inverted), lastState(0U), m_on(false)
{
}

void
Led::on ()
{
  m_blink = eLedState::ON;
}

void
Led::slowBlink ()
{
  m_blink = eLedState::SLOW_BLINK;
}

void
Led::fastBlink ()
{
  m_blink = eLedState::FAST_BLINK;
}

void
Led::off ()
{
  m_blink = eLedState::OFF;
}


void
Led::run ()
{
  if ((lastState % 3 == 0 && m_blink == eLedState::FAST_BLINK)
	  || (lastState % 15 == 0 && m_blink == eLedState::SLOW_BLINK))
    m_on = !m_on;
  else if (m_blink == eLedState::ON)
    m_on = true;
  else if (m_blink == eLedState::OFF)
    m_on = false;

  if(m_inverted)
	digitalWrite(m_pin , 1 - m_on);
  else
	digitalWrite(m_pin, m_on);
  lastState++;
}

/*--------------------------------------------------------------------- */
/*   HMI                                                                */
/*--------------------------------------------------------------------- */

HmiThread::HmiThread(uint16_t period):
    ledRGB(LED_RGB_R,LED_RGB_G,LED_RGB_B),
    led1(LED1),
    led2(LED2),
    led3(LED3),
    led4(LED4),
    ledDue_Rx(LED_DUE_RX, INVERTED),
    ledDue_Tx(LED_DUE_TX, INVERTED),
    ledDue_L(LED_DUE_L),
    start(BUTTON_START, HMI_DEBOUNCE, HMI_DEBOUNCE),
    matchColor(BUTTON_COLOR, HMI_DEBOUNCE, HMI_DEBOUNCE),
    user1(BUTTON_USER1, HMI_DEBOUNCE, HMI_DEBOUNCE),
    user2(BUTTON_USER2, HMI_DEBOUNCE, HMI_DEBOUNCE),
    m_period(period)
{

}

void
HmiThread::init()
{
  //create the thread
  g_ArdOs.createPeriodicThread_Cpp("HMI", *this, STACK_HMI, PRIO_HMI, PERIOD_HMI);
}

void
HmiThread::run()
{
    ledRGB.run();
    led1.run();
    led2.run();
    led3.run();
    led4.run();
    ledDue_Rx.run();
    ledDue_Tx.run();
    ledDue_L.run();
}
