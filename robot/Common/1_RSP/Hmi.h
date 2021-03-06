/*
 * GpioTools.h
 *
 *  Created on: 21 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_HMI_H_
#define ROBOTS_HMI_H_

#include <stdint.h>
#include "BSP.hpp"
#include "Actuators/Buzzer.h"
#include <Com.h>

namespace ard
{
    /*--------------------------------------------------------------------- */
    /*   OUTPUT                                                             */
    /*--------------------------------------------------------------------- */

    typedef enum
    {
        RED, GREEN, BLUE, YELLOW, CYAN, PURPLE, WHITE, MULTI, eRgb_Max /*Change colors regularly*/
    } eRgb;

    typedef enum
    {
        OFF, ON, SLOW_BLINK, FAST_BLINK, eLedState_Max
    } eLedState;

    class RgbLed
    {

    public:
        RgbLed(int pin_r, int pin_g, int pin_b);

        //Configure the displayed color and blink effect
        void set(eRgb color, eLedState blink);

        //Shut the led off
        void off();

        //Expected to be called periodically
        void
        run();

        uint8_t getState();

    private:

        void
        output(bool on);

        eRgb m_color;
        eLedState m_blink;
        PwmOutput pwm_r;
        PwmOutput pwm_g;
        PwmOutput pwm_b;
        uint8_t lastState;
        bool m_on;

    };

//utility to have explicit constructor
#define INVERTED 1

    class Led
    {
    public:
        //If the LED is on when voltage is low, use the INVERTED define as a second parameter.
        Led(int pin, bool inverted = false);

        //Shut the led on
        void
        on();

        void
        slowBlink();

        void
        fastBlink();

        //Shut the led off
        void
        off();

        //Expected to be called periodically
        void
        run();

        void set(eLedState state);

        //is true when the LED is light on
        bool getState();

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
    class HmiThread: public Thread
    {
    public:
        //@param timer : buzzer timer (shall be completely dedicated)
        HmiThread(DueTimer& timer);

        //Implements Thread : makes LED blinking
        void run() override;

        /* -----------
         * UiMsg
         * -------------
         */

        //answer true if the start is plugged
        bool isStartPlugged() const;

        //answer true when color switch is on the preferred color
        bool isColorSwitchOnPrefered() const;

        //Combines strategy switch to creates an integer. Left switch represent highest bit, Right one lowest one.
        uint8_t getStrategyId() const;

        //Drive the RGB LED
        void setRGBled(eRgb color, eLedState blink);

        //Drive the 4 green leds
        //for param led see BSH.h : LED1, LED2, LED3, LED4
        void setLed(uint8_t led, eLedState blink);

        RgbLed ledRGB;
        Led led1;
        Led led2;
        Led led3;
        Led led4;
        Led ledDue_Rx;
        Led ledDue_Tx;
        Led ledDue_L;

        FilteredInput tirette;
        FilteredInput matchColor;
        FilteredInput user1; //user1 is on the left (1/2 choice)
        FilteredInput user2; //user2 is on the right (test/master choice)

        Buzzer buzzer;

        apb_HmiState const& serealize();

    private:
        apb_HmiState state;
    };

} /* namespace ard */




#endif /* ROBOTS_HMI_H_ */
