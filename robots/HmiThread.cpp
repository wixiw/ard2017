/*
 * GpioTools.cpp
 *
 *  Created on: 21 oct. 2016
 *      Author: wix
 */

#include "HmiThread.h"

#include "K_pinout.h"
#include <Arduino.h>

using namespace ard;

/*--------------------------------------------------------------------- */
/*   INPUT                                                              */
/*--------------------------------------------------------------------- */

Button::Button (int pin, uint16_t debounce_ms)
{
  m_pin = pin;
  m_currentState = !digitalRead (m_pin);
  m_previousState = m_currentState; // Force new State
  m_timeChanged = millis ();
  m_debounce = debounce_ms;
}

void
Button::init ()
{
  pinMode (m_pin, INPUT);
}

bool
Button::newState ()
{
  bool ret = false;

  if (millis () - m_timeChanged >= m_debounce)
    {

      int curr = getCurrentState ();
      int readIn = digitalRead (m_pin);

      if (curr != readIn)
	{
	  m_previousState = curr;
	  m_currentState = readIn;
	  m_timeChanged = millis ();
	  ret = true;
	}
    }

  return ret;
}

bool
Button::getRawState ()
{
  return digitalRead (m_pin);
}

/*--------------------------------------------------------------------- */
/*   OUTPUT                                                             */
/*--------------------------------------------------------------------- */

RgbLed::RgbLed (int pin_r, int pin_g, int pin_b)
{
  m_r = pin_r;
  m_g = pin_g;
  m_b = pin_b;

  m_color = WHITE;
  m_blink = ON;

  lastState = 0;
  m_on = false;
}

void
RgbLed::init ()
{
  pinMode (m_r, OUTPUT);
  pinMode (m_g, OUTPUT);
  pinMode (m_b, OUTPUT);
  off();
}

void
RgbLed::set (rgb_color_t color, eLedState blink)
{
  m_color = color;
  m_blink = blink;
}

void
RgbLed::off ()
{
  m_blink = OFF;
}

void
RgbLed::run ()
{
  if ((lastState % 2 == 0 && m_blink == FAST_BLINK)
  	  || (lastState % 10 == 0 && m_blink == SLOW_BLINK))
    m_on = !m_on;
  else if (m_blink == ON)
    m_on = true;
  else if (m_blink == OFF)
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
	case RED:
	  digitalWrite (m_r, LOW);
	  digitalWrite (m_g, HIGH);
	  digitalWrite (m_b, HIGH);
	  break;
	case GREEN:
	  digitalWrite (m_r, HIGH);
	  digitalWrite (m_g, LOW);
	  digitalWrite (m_b, HIGH);
	  break;
	case BLUE:
	  digitalWrite (m_r, HIGH);
	  digitalWrite (m_g, HIGH);
	  digitalWrite (m_b, LOW);
	  break;
	case YELLOW:
	  digitalWrite (m_r, LOW);
	  digitalWrite (m_g, LOW);
	  digitalWrite (m_b, HIGH);
	  break;
	case CYAN:
	  digitalWrite (m_r, HIGH);
	  digitalWrite (m_g, LOW);
	  digitalWrite (m_b, LOW);
	  break;
	case PURPLE:
	  digitalWrite (m_r, LOW);
	  digitalWrite (m_g, HIGH);
	  digitalWrite (m_b, LOW);
	  break;
	case WHITE:
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
    m_blink (OFF), m_pin (pin), m_inverted(inverted), lastState(0U), m_on(false)
{
}

void
Led::init ()
{
  pinMode (m_pin, OUTPUT);
  //shut the led down
  off();
}

void
Led::on ()
{
  m_blink = ON;
}

void
Led::slowBlink ()
{
  m_blink = SLOW_BLINK;
}

void
Led::fastBlink ()
{
  m_blink = FAST_BLINK;
}

void
Led::off ()
{
  m_blink = OFF;
}


void
Led::run ()
{
  if ((lastState % 3 == 0 && m_blink == FAST_BLINK)
	  || (lastState % 15 == 0 && m_blink == SLOW_BLINK))
    m_on = !m_on;
  else if (m_blink == ON)
    m_on = true;
  else if (m_blink == OFF)
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
    start(BUTTON_START),
    matchColor(BUTTON_COLOR),
    user1(BUTTON_USER1),
    user2(BUTTON_USER2),
    m_period(period)
{

}

void
HmiThread::init()
{
  //create the thread
  g_ArdOs.createPeriodicThread_Cpp("HMI", *this, STACK_HMI, PRIO_HMI, PERIOD_HMI);

  ledRGB.init();
  led1.init();
  led2.init();
  led3.init();
  led4.init();
  ledDue_Rx.init();
  ledDue_Tx.init();
  ledDue_L.init();
  start.init();
  matchColor.init();
  user1.init();
  user2.init();
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
