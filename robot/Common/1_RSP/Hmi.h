/*
 * GpioTools.h
 *
 *  Created on: 21 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_HMI_H_
#define ROBOTS_HMI_H_

#include <stdint.h>
#include "Sensors/GpioTools.h"
#include "BSP.h"

namespace ard
{
    /*--------------------------------------------------------------------- */
    /*   OUTPUT                                                             */
    /*--------------------------------------------------------------------- */

    typedef enum
    {
        RED, GREEN, BLUE, YELLOW, CYAN, PURPLE, WHITE
    } eRgb;

    typedef enum
    {
        OFF, ON, SLOW_BLINK, FAST_BLINK
    } eLedState;

    class RgbLed
    {

    public:
        RgbLed(int pin_r, int pin_g, int pin_b);

        //Configure the displayed color and blink effect
        void
        set(eRgb color, eLedState blink);

        //Shut the led off
        void
        off();

        //Expected to be called periodically
        void
        run();

    private:

        void
        output(bool on);

        eRgb m_color;
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
        //@param period : thread period in ms
        //@param timer : buzzer timer (shall be completely dedicated)
        HmiThread(uint16_t period /*ms*/, DueTimer& timer);

        //Implements Thread : makes LED blinking
        void run() override;

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
    };

    //Music
    extern Tone melody_imperialMarch[10];

} /* namespace ard */

// NB: ALL NOTES DEFINED WITH STANDARD ENGLISH NAMES, EXCEPT FROM "A"
//THAT IS CALLED WITH THE ITALIAN NAME "LA" BECAUSE A0,A1...ARE THE ANALOG PINS ON ARDUINO.
// (Ab IS CALLED Ab AND NOT LAb)
#define C0 16.35
#define Db0 17.32
#define D0  18.35
#define Eb0 19.45
#define E0  20.60
#define F0  21.83
#define Gb0 23.12
#define G0  24.50
#define Ab0 25.96
#define LA0 27.50
#define Bb0 29.14
#define B0  30.87
#define C1  32.70
#define Db1 34.65
#define D1  36.71
#define Eb1 38.89
#define E1  41.20
#define F1  43.65
#define Gb1 46.25
#define G1  49.00
#define Ab1 51.91
#define LA1 55.00
#define Bb1 58.27
#define B1  61.74
#define C2  65.41
#define Db2 69.30
#define D2  73.42
#define Eb2 77.78
#define E2  82.41
#define F2  87.31
#define Gb2 92.50
#define G2  98.00
#define Ab2 103.83
#define LA2 110.00
#define Bb2 116.54
#define B2  123.47
#define C3  130.81
#define Db3 138.59
#define D3  146.83
#define Eb3 155.56
#define E3  164.81
#define F3  174.61
#define Gb3 185.00
#define G3  196.00
#define Ab3 207.65
#define LA3 220.00
#define Bb3 233.08
#define B3  246.94
#define C4  261.63
#define Db4 277.18
#define D4  293.66
#define Eb4 311.13
#define E4  329.63
#define F4  349.23
#define Gb4 369.99
#define G4  392.00
#define Ab4 415.30
#define LA4 440.00
#define Bb4 466.16
#define B4  493.88
#define C5  523.25
#define Db5 554.37
#define D5  587.33
#define Eb5 622.25
#define E5  659.26
#define F5  698.46
#define Gb5 739.99
#define G5  783.99
#define Ab5 830.61
#define LA5 880.00
#define Bb5 932.33
#define B5  987.77
#define C6  1046.50
#define Db6 1108.73
#define D6  1174.66
#define Eb6 1244.51
#define E6  1318.51
#define F6  1396.91
#define Gb6 1479.98
#define G6  1567.98
#define Ab6 1661.22
#define LA6 1760.00
#define Bb6 1864.66
#define B6  1975.53
#define C7  2093.00
#define Db7 2217.46
#define D7  2349.32
#define Eb7 2489.02
#define E7  2637.02
#define F7  2793.83
#define Gb7 2959.96
#define G7  3135.96
#define Ab7 3322.44
#define LA7 3520.01
#define Bb7 3729.31
#define B7  3951.07
#define C8  4186.01
#define Db8 4434.92
#define D8  4698.64
#define Eb8 4978.03


#endif /* ROBOTS_HMI_H_ */
