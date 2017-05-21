/*
 Copyright (c) 2013 Arduino LLC. All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#if defined(ARDUINO_ARCH_SAM)

#include "Servo.h"
#include "BSP.hpp"
#include "ArdOs.h"
#include "core/ArdMaths.h"

// Architecture specific include
#include "ServoTimers.h"

#define Servo_VERSION           2     // software version of this library

#define MIN_PULSE_WIDTH       500     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      2500     // the longest pulse sent to a servo
#define REFRESH_INTERVAL    20000     // minumim time to refresh servos in microseconds
#define MAX_SERVOS             12     // the maximum number of servos controlled by the library
#define INVALID_SERVO         255     // flag indicating an invalid servo index

#define usToTicks(_us)    (( clockCyclesPerMicrosecond() * _us) / 32)     // converts microseconds to tick
#define ticksToUs(_ticks) (( (unsigned)_ticks * 32)/ clockCyclesPerMicrosecond() ) // converts from ticks back to microseconds

static servo_t servos[MAX_SERVOS];                          // static array of servo structures
uint8_t ServoCount = 0;                                     // the total number of attached servos
static volatile int8_t currentServoId;                      // counter for the servo being pulsed for each timer (or -1 if refresh interval)

// convenience macros
#define SERVO_MIN() (MIN_PULSE_WIDTH - this->minArduino * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->maxArduino * 4)  // maximum value in uS for this servo

static uint32_t REFRESH_INTERVAL_US = usToTicks(REFRESH_INTERVAL); //pre-compute value used in interrupt for better performance

/************ static functions common to all instances ***********************/

//------------------------------------------------------------------------------
/// Interrupt handler for the TC0 channel 1.
//------------------------------------------------------------------------------
void HANDLER_FOR_TIMER1(void)
{
    //DEBUG_SET_HIGH(); //uncomment to check period and delay with oscilloscope

    // clear interrupt
    TC_FOR_TIMER1->TC_CHANNEL[CHANNEL_FOR_TIMER1].TC_SR;
    if (currentServoId < 0)
    {
        TC_FOR_TIMER1->TC_CHANNEL[CHANNEL_FOR_TIMER1].TC_CCR |= TC_CCR_SWTRG; // channel set to -1 indicated that refresh interval completed so reset the timer
    }
    else
    {
        if (currentServoId < ServoCount && servos[currentServoId].Pin.isActive == true)
        {
            ServoPin_t pin = servos[currentServoId].Pin;
            PIO_Clear(pin.port, pin.id);
        }
    }

    currentServoId++;    // increment to the next channel
    if ( currentServoId < ServoCount && currentServoId < MAX_SERVOS)
    {
        TC_FOR_TIMER1->TC_CHANNEL[CHANNEL_FOR_TIMER1].TC_RA = TC_FOR_TIMER1->TC_CHANNEL[CHANNEL_FOR_TIMER1].TC_CV + servos[currentServoId].ticks;
        // check if activated
        if (servos[currentServoId].Pin.isActive == true)
        {
            ServoPin_t pin = servos[currentServoId].Pin;
            PIO_Set(pin.port, pin.id);
        }
    }
    else
    {
        // finished all channels so wait for the refresh period to expire before starting over
        if ((TC_FOR_TIMER1->TC_CHANNEL[CHANNEL_FOR_TIMER1].TC_CV) + 4 < REFRESH_INTERVAL_US)
        { // allow a few ticks to ensure the next OCR1A not missed
            TC_FOR_TIMER1->TC_CHANNEL[CHANNEL_FOR_TIMER1].TC_RA = REFRESH_INTERVAL_US;
        }
        else
        {
            TC_FOR_TIMER1->TC_CHANNEL[CHANNEL_FOR_TIMER1].TC_RA = TC_FOR_TIMER1->TC_CHANNEL[CHANNEL_FOR_TIMER1].TC_CV + 4;  // at least REFRESH_INTERVAL has elapsed
        }
        currentServoId = -1; // this will get incremented at the end of the refresh period to start again at the first channel
    }

    //DEBUG_SET_LOW(); //uncomment to check period and delay with oscilloscope : default empty duration (2 io write + 1 function call) is 750ns
}

static void _initISR(Tc *tc, uint32_t channel, uint32_t id, IRQn_Type irqn)
{
    pmc_enable_periph_clk(id);
    TC_Configure(tc, channel,
    TC_CMR_TCCLKS_TIMER_CLOCK3 | // MCK/32
            TC_CMR_WAVE |                // Waveform mode
            TC_CMR_WAVSEL_UP_RC);       // Counter running up and reset when equals to RC

    /* 84MHz, MCK/32, for 1.5ms: 3937 */
    TC_SetRA(tc, channel, 2625); // 1ms

    /* Configure and enable interrupt */
    NVIC_EnableIRQ(irqn);
    // TC_IER_CPAS: RA Compare
    tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPAS;

    // Enables the timer clock and performs a software reset to start the counting
    TC_Start(tc, channel);
}


static boolean isTimerActive()
{
    // returns true if any servo is active on this timer
    for (uint8_t channel = 0; channel < MAX_SERVOS; channel++)
    {
        if (servos[channel].Pin.isActive == true)
            return true;
    }
    return false;
}

/****************** end of static functions ******************************/

Servo::Servo(int pin, uint16_t min, uint16_t max):
        servoIndex(0),
        minArduino(0),
        maxArduino(0),
        angularMin(min),
        angularMax(max),
        currentAngleCommand(500)
{
    if (ServoCount < MAX_SERVOS)
    {
        this->servoIndex = ServoCount++;                    // assign a servo index to this instance
        writeMicroseconds(1500);		//Do not delete, strange but required 21 may 2017
        if( 1000 < angularMin )
            angularMin = 1000;

        if( 1000 < angularMax )
            angularMax = 1000;

        this->servoIndex = attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    }
    else
    {
        this->servoIndex = INVALID_SERVO;  // too many servos
    }
}

uint8_t Servo::attach(int pin, int min, int max)
{
    //Return if too many servos have been instanciated
    if (MAX_SERVOS <= this->servoIndex)
    {
        return INVALID_SERVO;
    }

    //Check if pin is not already used
    for (int i = 0; i < ServoCount; i++)
    {
        if (i != this->servoIndex && servos[i].Pin.nbr == pin)
        {
            return INVALID_SERVO;
        }
    }

    pinMode(pin, OUTPUT); // set servo pin to output
    servos[this->servoIndex].Pin.nbr = pin;
    servos[this->servoIndex].Pin.port = g_APinDescription[pin].pPort;
    servos[this->servoIndex].Pin.id = g_APinDescription[pin].ulPin;
    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
    this->minArduino = (MIN_PULSE_WIDTH - min) / 4; //resolution of min/max is 4 uS
    this->maxArduino = (MAX_PULSE_WIDTH - max) / 4;

    return this->servoIndex;
}

bool Servo::enable()
{
    if( this->servoIndex == INVALID_SERVO )
        return false;

    // initialize the timer if it has not already been initialized
    if (isTimerActive() == false)
    {
        _initISR(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1, ID_TC_FOR_TIMER1, IRQn_FOR_TIMER1);
    }
    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
    return true;
}

void Servo::disable()
{
    servos[this->servoIndex].Pin.isActive = false;
    if (isTimerActive() == false)
    {
        TC_Stop(TC_FOR_TIMER1, CHANNEL_FOR_TIMER1);
    }
}

void Servo::goTo(uint16_t value)
{
    if (value < angularMin)
        value = angularMin;
    else if (value > angularMax)
        value = angularMax;

    //if servo is not enabled, enable it
    if( !servos[this->servoIndex].Pin.isActive )
        enable();

    //convert into us command
    value = perThousandToUs(value);
    writeMicroseconds(value);
}

uint16_t Servo::perThousandToUs(uint16_t value)
{
    return map(value, 0, 1000, SERVO_MIN(), SERVO_MAX());
}

uint16_t Servo::usToPerThousand(uint16_t value)
{
    return map(value, SERVO_MIN(), SERVO_MAX(), 0, 1000);
}

void Servo::writeMicroseconds(int value)
{
    // calculate and store the values for the given channel
    byte channel = this->servoIndex;
    if ((channel < MAX_SERVOS))   // ensure channel is valid
    {
        if (value < SERVO_MIN())          // ensure pulse width is valid
            value = SERVO_MIN();
        else if (value > SERVO_MAX())
            value = SERVO_MAX();
		
		currentAngleCommand = usToPerThousand(value);
        value = usToTicks(value);  // convert to ticks after compensating for interrupt overhead
        servos[channel].ticks = value;     
    }
}

int Servo::readMicroseconds()
{
    unsigned int pulsewidth;
    if (this->servoIndex != INVALID_SERVO)
        pulsewidth = ticksToUs(servos[this->servoIndex].ticks);
    else
        pulsewidth = 0;

    return pulsewidth;
}


#endif // ARDUINO_ARCH_SAM

