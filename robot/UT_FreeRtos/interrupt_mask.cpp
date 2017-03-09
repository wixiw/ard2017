/*
 * ArdSignal.cpp
 *
 *  Created on: 30 janv. 2017
 *      Author: wix
 */

// Test to determine if interrupt masks works properly
// The "L" led shall never stop blink ing (priority above OS)
// The "TX" led shall stop blinking during 3s when the thread enters critical section

#include "Arduino.h"
#include "ArdOs.h"
#include "BSP.h"

using namespace ard;

uint8_t trigger = 0;
uint8_t trigger2 = 1;

/** calibration factor for delayMS */
#define CAL_FACTOR (F_CPU/7000)
/** delay between led error flashes
 * \param[in] millis milliseconds to delay
 */
static void delayMS(uint32_t millis) {
  uint32_t iterations = millis * CAL_FACTOR;
  uint32_t i;
  for(i = 0; i < iterations; ++i) {
    asm volatile("nop\n\t");
  }
}

void ledInterrupt()
{
    trigger = 1 - trigger;
    digitalWrite(LED_DUE_L, trigger);
}

void led2Interrupt()
{
    trigger2 = 1 - trigger2;
    digitalWrite(LED_DUE_TX, trigger2);
}

//------------------------------------------------------------------------------
// high priority thread to set pin low
class ItMaskThread: public Thread
{
public:
    ItMaskThread():Thread("ItMask", tskIDLE_PRIORITY + 1, STACK_UNIT_TEST){};

    void run() override
    {
        while(1)
        {
            delayMS(1000);
            ArdOs::disableAllInterrupts();
            delayMS(1000);
            ArdOs::enableAllInterrupts();
        }
    }
};


//It's VERY important to have those variables out of the main scope, else,
//they will be destructed when Freertos intialize and destruct the first stack to replace it by OS thread stacks
ItMaskThread consThread;

void interrupt_mask()
{
    Timer6.setInterruptPriority(PRIORITY_IRQ_SYSCALL - 1);
    Timer7.setInterruptPriority(PRIORITY_IRQ_SYSCALL + 1);
    Timer6.attachInterrupt(ledInterrupt);
    Timer7.attachInterrupt(led2Interrupt);
    Timer6.start(500000);
    Timer7.start(500000);
    ArdOs::init();
    ArdOs::start();
}

