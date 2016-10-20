/*
 * ArdOs.cpp
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#include "ArdOs.h"
using namespace ard;

#define HEARTBEAT_PIN 72

//-------------------------------------------------------------------------------

//infinite wait signal
Signal infinite;

//singleton instanciation
ArdOs ArdOs::instance = ArdOs ();

//helper to prevent user from exiting their threads, as it push FreeRtos in assert
void
ArdOs_genericRun (void* pvParameters)
{
  ArdOs::genericRunParams* params = reinterpret_cast<ArdOs::genericRunParams*> (pvParameters);
  ardAssert(params != NULL, "Generic params cast failed.");

  //Informs that the task is started
  char taskName[configMAX_TASK_NAME_LEN];
  ard_getTaskName (taskName);
  Serial.println (String ("[ArdOs] ") + taskName + " is running.");

  //Run either the function or the class run method
  if(params->pClass)
    params->pClass->run();
  else
    params->method();

  //Wait infinitly so that the thread context is never exited (else FreeRtos would asserts)
  Serial.println (String ("[ArdOs] ") + taskName + " is finished.");
  infinite.wait ();
}

ArdOs::ArdOs ()
{
  nextThreadRank = 0;
  heartbeatCounter = 0;
  heartbeatPinValue = 0;
  INIT_TABLE_TO_ZERO(threads);
  INIT_TABLE_TO_ZERO(params);
}

void
ArdOs::init ()
{
  Serial.begin (115200);
  infinite.init ();
  pinMode (13, OUTPUT);
  pinMode (HEARTBEAT_PIN, OUTPUT);
}

void
ArdOs::start ()
{

  digitalWrite (13, LOW);
  digitalWrite (HEARTBEAT_PIN, heartbeatPinValue);

  // start FreeRTOS
  Serial.println (String ("[ArdOs] ") + "Start scheduler.");
  vTaskStartScheduler ();

  // should never reach this as the vTaskStartScheduler never ends
  Serial.println ("ERROR : Scheduler exited !");
  while (1)
    {
    };
}

void
ArdOs::kickHeartbeat ()
{
  ++heartbeatCounter;
  if (0 == heartbeatCounter % 100000)
    {
      //Serial.println("Hearbeat toggle");
      digitalWrite (HEARTBEAT_PIN, 1 - heartbeatPinValue);
      heartbeatPinValue = 1 - heartbeatPinValue;
    }
}

void
ArdOs::createThread_C (const char * const name, ThreadRunFct runFunction,
		     uint16_t stack, uint16_t priority)
{

  //Check inputs
  ardAssert(nextThreadRank < configMAX_PRIORITIES, "Too many threads.");
  ardAssert(priority < 100,
	    "priority is too high, check for stack/priority order.");

  //fill the params
  params[nextThreadRank].pClass = NULL;
  params[nextThreadRank].method = runFunction;

  //create the thread
  ardAssert(
      pdPASS == xTaskCreate(ArdOs_genericRun, name, stack, reinterpret_cast<void*>(&params[nextThreadRank]), priority, threads[nextThreadRank]),
      "Task creation failed.");

  //increment the table index
  ++nextThreadRank;
}

void
ArdOs::createThread_Cpp (const char * const name, IThread& pClass,
		     uint16_t stack, uint16_t priority)
{

  //Check inputs
  ardAssert(nextThreadRank < configMAX_PRIORITIES, "Too many threads.");
  ardAssert(priority < 100,
	    "priority is too high, check for stack/priority order.");

  //fill the params
  params[nextThreadRank].pClass = &pClass;
  params[nextThreadRank].method = NULL;

  //create the thread
  ardAssert(
      pdPASS == xTaskCreate(ArdOs_genericRun, name, stack, reinterpret_cast<void*>(&params[nextThreadRank]), priority, threads[nextThreadRank]),
      "Task creation failed.");

  //increment the table index
  ++nextThreadRank;
}

//-------------------------------------------------------------------------------

SwTimer::SwTimer () :
    m_entryDate (0U), m_delay (0U), m_started (false)
{
}

void
SwTimer::arm (uint32_t delayInMs)
{
  ardAssert(delayInMs != 0, "Delay shall be non null.");
  m_entryDate = millis ();
  m_delay = delayInMs;
  m_started = true;
}

void
SwTimer::cancel ()
{
  m_started = false;
  m_delay = 0U;
  m_entryDate = 0U;
}

bool
SwTimer::isFired () const
{
  if (m_started && m_delay <= millis () - m_entryDate)
    return true;
  else
    return false;
}

//-------------------------------------------------------------------------------

Mutex::Mutex () :
    m_lock (0)
{
}

bool
Mutex::init ()
{
  m_lock = xSemaphoreCreateMutex();
  if (m_lock == 0)
    return false;
  else
    return true;
}

void
Mutex::acquire ()
{
  ardAssert(m_lock != 0, "Mutex not initialized.");
  xSemaphoreTake(m_lock, portMAX_DELAY);
}

void
Mutex::release ()
{
  ardAssert(m_lock != 0, "Mutex not initialized.");
  xSemaphoreGive(m_lock);
}

//-------------------------------------------------------------------------------

Signal::Signal ()
{
  sem = 0;
}

bool
Signal::init ()
{
  sem = xSemaphoreCreateBinary();
  xSemaphoreTake(sem, 0);
  return sem != 0;
}

void
Signal::wait ()
{
  ardAssert(sem != 0, "Signal not initialized.");
  xSemaphoreTake(sem, portMAX_DELAY);
}

void
Signal::set ()
{
  ardAssert(sem != 0, "Signal not initialized.");
  xSemaphoreGive(sem);
}

void
Signal::setFromIsr ()
{
  ardAssert(sem != 0, "Signal not initialized.");
  portBASE_TYPE xHigherPriorityTaskWoken = 0;
  xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
}
