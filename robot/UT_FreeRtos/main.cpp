// Test to determine context switch time with a semaphore
// Connect a scope to pin 13
// Measure difference in time between first pulse with no context switch
// and second pulse started in ledControl and ended in ledOffTask.
// This is the time for the semaphore and a context switch.
#include "Arduino.h"
#include "FreeRTOS_ARM.h"

#define LED_PIN 13
// Semaphore to trigger context switch
SemaphoreHandle_t xSemaphore;
//------------------------------------------------------------------------------
// high priority thread to set pin low
static void ledOffTask(void *pvParameters) {
	for (;;) {
		xSemaphoreTake(xSemaphore, portMAX_DELAY);
		digitalWrite(LED_PIN, LOW);
		Serial.println("Low");
	}
}
//------------------------------------------------------------------------------
// lower priority thread to toggle LED and trigger thread 1
static void ledControl(void *pvParameters) {
	for (;;) {
		// first pulse to get time with no context switch
		digitalWrite(LED_PIN, HIGH);
		digitalWrite(LED_PIN, LOW);

		// start second pulse
		digitalWrite(LED_PIN, HIGH);
		Serial.println("High");

		// trigger context switch for task that ends pulse
		xSemaphoreGive(xSemaphore);

		// sleep until next tick
		vTaskDelay(1);
	}
}
//------------------------------------------------------------------------------
void setup() {
	Serial.begin(115200);
	pinMode(LED_PIN, OUTPUT);

	// create high priority thread
	xTaskCreate(ledOffTask,
	"Task1",
	configMINIMAL_STACK_SIZE,
	NULL,
	tskIDLE_PRIORITY + 2,
	NULL);

	// create lower priority thread
	xTaskCreate(ledControl,
	"Task2",
	configMINIMAL_STACK_SIZE,
	NULL,
	tskIDLE_PRIORITY + 1,
	NULL);

	// create semaphore
	vSemaphoreCreateBinary(xSemaphore);

	// start FreeRTOS
	vTaskStartScheduler();

	// should never return
	Serial.println("Die");
	while(1);
}
//------------------------------------------------------------------------------
void loop() {
	// Not used - idle loop has a very small, configMINIMAL_STACK_SIZE, stack
	// loop must never block
}

#define ARDUINO_MAIN

/*
 * Cortex-M3 Systick IT handler
 */
/*
extern void SysTick_Handler( void )
{
  // Increment tick count each ms
  TimeTick_Increment() ;
}
*/

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

/*
 * \brief Main entry point of Arduino application
 */
int main( void )
{
	// Initialize watchdog
	watchdogSetup();

	init();

	initVariant();

	delay(1);

#if defined(USBCON)
	USBDevice.attach();
#endif

	setup();

	for (;;)
	{
		loop();
		if (serialEventRun) serialEventRun();
	}

	return 0;
}