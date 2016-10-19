/*
 * ArdOs.cpp
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#include "ArdOs.h"
#include <Arduino.h>
using namespace ard;

#define HEARTBEAT_PIN 72

//-------------------------------------------------------------------------------

//singleton instanciation
ArdOs ArdOs::instance = ArdOs();

ArdOs::ArdOs()
{
	nextThreadRank = 0;
	heartbeatCounter = 0;
	heartbeatPinValue = 0;
	for(int i=0; i<configMAX_PRIORITIES; ++i)
	{
		threads[i] = NULL;
	}
}

void ArdOs::init() {

	Serial.begin(115200);
}

void ArdOs::start() {

	pinMode(13, OUTPUT);
	pinMode(HEARTBEAT_PIN, OUTPUT);
	digitalWrite(13, LOW);
	digitalWrite(HEARTBEAT_PIN, instance.heartbeatPinValue);

	// start FreeRTOS
	Serial.println("Start scheduler.");
	vTaskStartScheduler();

	// should never reach this as the vTaskStartScheduler never ends
	Serial.println("ERROR : Scheduler exited !");
	while (1) {
	};
}

void ArdOs::kickHeartbeat() {
	++instance.heartbeatCounter;
	if(0 == instance.heartbeatCounter%1000000)
	{
		//Serial.println("Hearbeat toggle");
		digitalWrite(HEARTBEAT_PIN, 1 - instance.heartbeatPinValue);
		instance.heartbeatPinValue = 1 - instance.heartbeatPinValue;
	}
}

void ArdOs::createThread(const char * const name, TaskFunction_t runFunction,
		uint16_t stack, uint16_t priority) {
	configASSERT(instance.nextThreadRank < configMAX_PRIORITIES);
	configASSERT(
			pdPASS == xTaskCreate(runFunction, NULL, stack, NULL, priority, instance.threads[instance.nextThreadRank]));
	++instance.nextThreadRank;
}

//-------------------------------------------------------------------------------

SwTimer::SwTimer() :
		m_entryDate(0U), m_delay(0U), m_started(false) {
}

void SwTimer::arm(uint32_t delayInMs) {
	configASSERT(delayInMs != 0);
	m_entryDate = millis();
	m_delay = delayInMs;
	m_started = true;
}

void SwTimer::cancel() {
	m_started = false;
	m_delay = 0U;
	m_entryDate = 0U;
}

bool SwTimer::isFired() const {
	if (m_started && m_delay <= millis() - m_entryDate)
		return true;
	else
		return false;
}

//-------------------------------------------------------------------------------

Mutex::Mutex() :
		m_lock(0) {
}

bool Mutex::init() {
	m_lock = xSemaphoreCreateMutex();
	if (m_lock == 0)
		return false;
	else
		return true;
}

void Mutex::acquire() {
	configASSERT(m_lock != 0);
	xSemaphoreTake(m_lock, portMAX_DELAY);
}

void Mutex::release() {
	configASSERT(m_lock != 0);
	xSemaphoreGive(m_lock);
}

//-------------------------------------------------------------------------------

Signal::Signal() {
	sem = 0;
}

bool Signal::init() {
	sem = xSemaphoreCreateBinary();
	xSemaphoreTake(sem, 0);
	return sem != 0;
}

void Signal::wait() {
	configASSERT(sem != 0);
	xSemaphoreTake(sem, portMAX_DELAY);
}

void Signal::set() {
	configASSERT(sem != 0);
	xSemaphoreGive(sem);
}

void Signal::setFromIsr() {
	configASSERT(sem != 0);
	portBASE_TYPE xHigherPriorityTaskWoken = 0;
	xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
}
