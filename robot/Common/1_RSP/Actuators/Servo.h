/*
 Servo.h - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* 
 A servo is activated by creating an instance of the Servo class passing
 the desired pin to the attach() method.
 The servos are pulsed in the background using the value most recently
 written using the write() method.

 Note that analogWrite of PWM on pins associated with the timer are
 disabled when the first servo is attached.
 Timers are seized as needed in groups of 12 servos - 24 servos use two
 timers, 48 servos will use four.
 The sequence used to sieze timers is defined in timers.h
 */

#ifndef Servo_h
#define Servo_h
#include "sam3x8e.h"
#include <inttypes.h>

/* 
 * Defines for 16 bit timers used with  Servo library 
 *
 * If _useTimerX is defined then TimerX is a 16 bit timer on the current board
 * timer16_Sequence_t enumerates the sequence that the timers should be allocated
 * _Nbr_16timers indicates how many 16 bit timers are available.
 */

typedef struct
{
    uint8_t nbr :6;             // a pin number from 0 to 63
    uint8_t isActive :1;             // true if this channel is enabled, pin not pulsed if false
    Pio* port;                          //GPIO port in which the pin is located
    uint32_t id;                        //id of the pin in the port (from 0 to 31)
} ServoPin_t;

typedef struct
{
    ServoPin_t Pin;
    volatile unsigned int ticks;
} servo_t;

class IServo
{
public:
    virtual ~IServo() = default;

    //Disable the power
    virtual void disable() = 0;

    // set servo position between 0 and 1000 per thousand of min/max position (0 is min), and enable the power
    virtual void goTo(uint16_t value) = 0;

    // returns servo position between 0 and 1000 per thousand of min/max position (0 is min)
    virtual uint16_t read() = 0;

};

class Servo: public IServo
{
public:
    //ARD addon : Set minimal angular commands in per thousand (SW limits)
    Servo(int pin, uint16_t min = 0U, uint16_t max = 1000U);

    //Implements IServo
    void disable() override;
    bool enable();  //Start generating the PWM. return false is the servo has an invalid configuration.

    //Implements IServo
    void goTo(uint16_t value) override;

    //Implements IServo
    uint16_t read() override {return currentAngleCommand;};

    // Write pulse width in microseconds, in general you will prefer goTo
    void writeMicroseconds(int value);

    //Convert a [0;1000] command into a micro seconds command
    uint16_t perThousandToUs(uint16_t value);
    uint16_t usToPerThousand(uint16_t value);
private:


    int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
    uint8_t attach(int pin, int min, int max); // as above but also sets min and max values for writes. //ARD : function made private as min/max management is unclear
    uint8_t servoIndex;               // index into the channel data for this servo
    uint16_t minArduino;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH
    uint16_t maxArduino;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH
    uint16_t angularMin;               // ARD addons : minimal angular command in per thousand (i.e. SW limit), default 0
    uint16_t angularMax;               // ARD addons : maximal angular command in per thousand (i.e. SW limit), default 1000
    uint16_t currentAngleCommand;
};

#endif
