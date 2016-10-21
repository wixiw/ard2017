/*
 * GpioTools.h
 *
 *  Created on: 21 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_HMITHREAD_H_
#define ROBOTS_HMITHREAD_H_

#include <stdint.h>
#include "ArdOs.h"

namespace ard
{
  /*--------------------------------------------------------------------- */
  /*   INPUT                                                              */
  /*--------------------------------------------------------------------- */

  /**
   * Create a "Button" filter on an input pin.
   * It's a debounce filter.
   */
  class Button
  {
  public:
    Button (int pin, uint16_t debounce_ms = 200 /* ms */);

    //Configure the pin as an input
    void
    init ();

    bool
    newState ();

    bool
    getPreviousState ()
    {
      return m_previousState;
    }
    ;

    bool
    getCurrentState ()
    {
      return m_currentState;
    }
    ;

    //@returns un-filtered input
    bool
    getRawState ();

  private:
    int m_pin;
    bool m_previousState;
    bool m_currentState;
    unsigned long m_timeChanged;
    uint16_t m_debounce; //in ms
  };

  /*--------------------------------------------------------------------- */
  /*   OUTPUT                                                             */
  /*--------------------------------------------------------------------- */

  typedef enum
  {
    RED, GREEN, BLUE, YELLOW, CYAN, PURPLE, WHITE
  } rgb_color_t;

  typedef enum
  {
    OFF, ON, SLOW_BLINK, FAST_BLINK
  } eLedState;

  class RgbLed
  {

  public:
    RgbLed (int pin_r, int pin_g, int pin_b);

    //Expected to be called before any action
    //configure the pins
    void
    init ();

    //Configure the displayed color and blink effect
    void
    set (rgb_color_t color, eLedState blink);

    //Shut the led off
    void
    off ();

    //Expected to be called periodically
    void
    run ();

  private:

    void
    output (bool on);

    rgb_color_t m_color;
    eLedState m_blink;
    int m_r;
    int m_g;
    unsigned int m_b;
    uint8_t lastState;
    bool m_on;

  };

  //utility to have explicit constructor
#define INVERTED 1
  class Led
  {
  public:
    //If the LED is on when voltage is low, use the INVERTED define as a second parameter.
    Led (int pin, bool inverted = false);

    //Expected to be called before any action
    //configure the pins
    void
    init ();

    //Shut the led on
    void
    on ();

    void
    slowBlink ();

    void
    fastBlink ();

    //Shut the led off
    void
    off ();

    //Expected to be called periodically
    void
    run ();

  private:
    eLedState m_blink;
    int m_pin;
    bool m_inverted; //is true when high voltage shut the led down
    uint8_t lastState;
    bool m_on;
  };

  /*--------------------------------------------------------------------- */
  /*   HMI                                                                */
  /*--------------------------------------------------------------------- */

  /**
   * This class is an assembly, so it provides public members to prevent
   * writing non const getters. It's also a thread so that HMI can still
   * be reactive if the robot is bullshitting (you should set a quite high priority)
   *
   * The ledDue_L should not be used, the OS is using it to display errors
   */
  class HmiThread: public IThread
  {
  public:
    HmiThread (uint16_t period /*ms*/);

    void
    init ();

    void
    run ();

    RgbLed ledRGB;
    Led led1;
    Led led2;
    Led led3;
    Led led4;
    Led ledDue_Rx;
    Led ledDue_Tx;
    Led ledDue_L;

    Button start;
    Button matchColor;
    Button user1;
    Button user2;

  private:
    uint16_t m_period; //in ms
  };

} /* namespace ard */

#endif /* ROBOTS_HMITHREAD_H_ */
