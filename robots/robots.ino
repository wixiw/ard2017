/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "ArdOs.h"

using namespace ard;

//Signal s1;

/**
 * Test procedure:
 *
 * "Task2 hello (shall apear first)" shall appear before "Task1 hello" (testing  priorities)
 * "Task1 hello" shall appear several time during the Task2 1ms sleep (testing that delay makes Task2 sleep, so that Task1 canr un even if at lower priority)
 * "Task2 sleep finished, waiting Task1()"
 *
 */

//------------------------------------------------------------------------------
// high priority thread to set pin low
static void Task1(void *pvParameters) {
	for (;;) {
		Serial.println("Task1 hello");
	}
}

//------------------------------------------------------------------------------
// high priority thread to set pin low
static void Task2(void *pvParameters) {
	Serial.println("Task2 hello (shall apear first)");
//	delay(1);
//	Serial.println("Task2 sleep finished, waiting Task1()");
//	s1.wait();
//	Serial.println("Task2 awaken");
	for (;;) {
		delay(1000);
	}
}


//------------------------------------------------------------------------------
void setup() {
	g_ArdOs.init();

//	// create high priority thread
//	xTaskCreate(Task1, "Task1", configMINIMAL_STACK_SIZE, NULL,
//			tskIDLE_PRIORITY + 1, NULL);
//
//	xTaskCreate(Task2, "Task2", configMINIMAL_STACK_SIZE, NULL,
//			tskIDLE_PRIORITY + 2, NULL);

	g_ArdOs.start();
}

//------------------------------------------------------------------------------
void loop() {
	// idle loop has a very small, configMINIMAL_STACK_SIZE, stack
	// loop must never block
	// the heartbeat movement is a proof that the CPU is not overused as the minimal priority task is executed
	g_ArdOs.kickHeartbeat();
}
