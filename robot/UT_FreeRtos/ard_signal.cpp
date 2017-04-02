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
Signal ledSignal;

//------------------------------------------------------------------------------
// high priority thread to set pin low
class LedOffThread: public Thread
{
public:
    LedOffThread():Thread("TaskHi", tskIDLE_PRIORITY + 3, STACK_UNIT_TEST){};

    void run() override
    {
        for (;;)
        {
            ledSignal.wait();
            //xSemaphoreTake(xSemaphore2, portMAX_DELAY);
            digitalWrite(LED_PIN, LOW);
        }
    }
};

//------------------------------------------------------------------------------
// lower priority thread to toggle LED and trigger thread 1
class LedOnThread: public Thread
{
public:
    LedOnThread():Thread("TaskLo", tskIDLE_PRIORITY + 2, STACK_UNIT_TEST){};

    void run() override
    {
        // first pulse to get time with no context switch
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(LED_PIN, LOW);

        for (;;)
        {
            // start second pulse
            digitalWrite(LED_PIN, HIGH);

            // trigger context switch for task that ends pulse
            ledSignal.set();
            //xSemaphoreGive(xSemaphore2);
        }
    }
};

//It's VERY important to have those variables out of the main scope, else,
//they will be destructed when Freertos intialize and destruct the first stack to replace it by OS thread stacks
LedOnThread ledOnThread;
LedOffThread ledOffThread;

void ard_signal()
{
    pinMode(LED_PIN, OUTPUT);

    ArdOs::init();
    ArdOs::start();
}

