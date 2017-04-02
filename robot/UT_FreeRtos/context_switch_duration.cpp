/*
 * context_switch_duration.cpp
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
#include "FreeRTOS_ARM.h"
#include "BSP.hpp"

#define LED_PIN 13
// Semaphore to trigger context switch
SemaphoreHandle_t xSemaphore;
//------------------------------------------------------------------------------
// high priority thread to set pin low
static void ledOffTask(void *pvParameters) {
	for (;;) {
		xSemaphoreTake(xSemaphore, portMAX_DELAY);
        //vTaskSuspend(NULL);
		digitalWrite(LED_PIN, LOW);
	}
}
//------------------------------------------------------------------------------
// lower priority thread to toggle LED and trigger thread 1
static void ledControl(void *pvParameters) {
		// first pulse to get time with no context switch
		digitalWrite(LED_PIN, HIGH);
		digitalWrite(LED_PIN, LOW);

	for (;;) {
		// start second pulse
		digitalWrite(LED_PIN, HIGH);

		// trigger context switch for task that ends pulse
		xSemaphoreGive(xSemaphore);
	}
}

void context_switch_duration()
{
    //Init drivers
    init_bsp();

	pinMode(LED_PIN, OUTPUT);

	// create high priority thread
	xTaskCreate(ledOffTask,
	"Task1",
	1000,
	NULL,
	tskIDLE_PRIORITY + 2,
	NULL);

	// create lower priority thread
	xTaskCreate(ledControl,
	"Task2",
	1000,
	NULL,
	tskIDLE_PRIORITY + 1,
	NULL);

	// create semaphore
	xSemaphore = xSemaphoreCreateBinary();

    // start FreeRTOS
    vTaskStartScheduler();
}


