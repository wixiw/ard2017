/*
 * ArdSignal.cpp
 *
 *  Created on: 30 janv. 2017
 *      Author: wix
 */

// Test to determine context switch time with a semaphore
// Connect a scope to pin 13
// Measure difference in time between first pulse with no context switch
// and second pulse started in ledControl and ended in ledOffTask.
// This is the time for the semaphore and a context switch.
#include "Arduino.h"
#include "ArdOs.h"
#include "BSP.hpp"

using namespace ard;

#define LED_PIN 13
// Signal to trigger context switch
Semaphore sem(3, 1);

//------------------------------------------------------------------------------
// high priority thread to set pin low
class ProducerThread: public Thread
{
public:
    ProducerThread():Thread("Prod", tskIDLE_PRIORITY + 1, STACK_UNIT_TEST){};

    void run() override
    {
        for (;;)
        {
            digitalWrite(LED_PIN, LOW);
            sem.give();
        }
    }
};

//------------------------------------------------------------------------------
// lower priority thread to toggle LED and trigger thread 1
class ConsummerThread: public Thread
{
public:
    ConsummerThread():Thread("Cons", tskIDLE_PRIORITY + 2, STACK_UNIT_TEST){};

    void run() override
    {
        for (;;)
        {
            sem.take();
            digitalWrite(LED_PIN, HIGH);
        }
    }
};

//It's VERY important to have those variables out of the main scope, else,
//they will be destructed when Freertos intialize and destruct the first stack to replace it by OS thread stacks
ConsummerThread consThread;
ProducerThread prodThread;

void ard_semaphore()
{
    pinMode(LED_PIN, OUTPUT);

    ArdOs::init();
    ArdOs::start();
}

