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

//singleton instanciation
ArdOs ArdOs::instance = ArdOs ();

//helper to prevent user from exiting their threads, as it push FreeRtos in assert
void
ArdOs_genericRun (void* pvParameters)
{
  ArdOs::genericRunParams* params =
      reinterpret_cast<ArdOs::genericRunParams*> (pvParameters);
  ardAssert(params != NULL, "Generic params cast failed.");

  //Informs that the task is started
  char taskName[configMAX_TASK_NAME_LEN];
  ard_getTaskName (taskName);
  Serial.println (String ("[ArdOs] ") + taskName + " is running.");

  //The thread is periodic
  if (params->period)
    {
      TickType_t lastWakeTime = xTaskGetTickCount();
      while(2)/* because 1 is has-been*/
      {
	  //Run either the function or the class run method
	  if (params->pClass)
	    params->pClass->run ();
	  else
	    params->method ();

	  //wait until next period
	  vTaskDelayUntil( &lastWakeTime, params->period );
      }
    }
  //The thread isnot periodic
  else
    {
      //Run either the function or the class run method
      if (params->pClass)
	params->pClass->run ();
      else
	params->method ();

      //Wait infinitly so that the thread context is never exited (else FreeRtos would asserts)
      Serial.println (String ("[ArdOs] ") + taskName + " is finished.");
      g_ArdOs.Signal_wait(infinite);
    }

}

ArdOs::ArdOs ()
{
  nextThreadRank = 0;
  heartbeatCounter = 0;
  heartbeatPinValue = 0;
  signalCount = 0;
  mutexCount = 0;
  state = UNINIT;
  bootDuration = 0;
  INIT_TABLE_TO_ZERO(threads);
  INIT_TABLE_TO_ZERO(params);
}

void
ArdOs::init ()
{
  ardAssert(state == UNINIT, "ArdOs is not in the right state to do an init");
  infinite = Signal_create ();
  pinMode (13, OUTPUT);
  pinMode (HEARTBEAT_PIN, OUTPUT);
  state = INITIALIZED;
}

void
ArdOs::start ()
{
  ardAssert(state == INITIALIZED,
	    "ArdOs is not in the right state to do a start");

  digitalWrite (13, LOW);
  digitalWrite (HEARTBEAT_PIN, heartbeatPinValue);

  // start FreeRTOS
  state = RUNNING;
  bootDuration = millis ();
  Serial.println (
      String ("[ArdOs] ") + "Robot is booted, it took " + bootDuration
	  + " ms.");
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
  ardAssert(state == RUNNING, "ArdOs shall be running to kickHeartbeat");

  ++heartbeatCounter;
  if (0 == heartbeatCounter % 100000)
    {
      //Serial.println("Hearbeat toggle");
      digitalWrite (HEARTBEAT_PIN, 1 - heartbeatPinValue);
      heartbeatPinValue = 1 - heartbeatPinValue;
    }
}

void
ArdOs::displayStats ()
{
  char text[40 * configMAX_PRIORITIES];
  vTaskList (text);
  Serial.println ("--------------- ArdOs Stats  ------------------");
  Serial.println ("|   Thread   | State | Prio | Free stack | ID |");
  Serial.println ("-----------------------------------------------");
  Serial.print (text);
  Serial.println (
      " * States : blocked ('B'), ready ('R'), deleted ('D') or suspended ('S').");
  Serial.println (" * Priority : higher number, higher priority");
  Serial.println ("-----------------------------------------------");
  Serial.println (
      String ("Nb Threads : ") + String (nextThreadRank + 2) + " / "
	  + String(configMAX_PRIORITIES + 2));
  Serial.println (String ("Nb Mutexes : ") + mutexCount);
  Serial.println (String ("Nb Signals : ") + signalCount);
  Serial.println (String ("Booted in ") + bootDuration + " ms.");
  Serial.println ("-----------------------------------------------");

  //TODO static reportStackSizes
  //TODO static reportCpuConsumption
}

void
ArdOs::createThread_C (const char * const name, ThreadRunFct runFunction,
		       uint16_t stack, uint16_t priority)
{
  ardAssert(state == UNINIT,
	    "ArdOs is not in the right state to do a thread creation");

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
  createPeriodicThread_Cpp (name, pClass, stack, priority, 0);
}

void
ArdOs::createPeriodicThread_Cpp (const char * const name, IThread& pClass,
				 uint16_t stack, uint16_t priority,
				 uint16_t periodMs)
{
  ardAssert(state == UNINIT,
	    "ArdOs is not in the right state to do a thread creation");

  //Check inputs
  ardAssert(nextThreadRank < configMAX_PRIORITIES, "Too many threads.");
  ardAssert(priority < 100,
	    "priority is too high, check for stack/priority order.");

  //fill the params
  params[nextThreadRank].pClass = &pClass;
  params[nextThreadRank].method = NULL;
  params[nextThreadRank].period = periodMs;

  //create the thread
  ardAssert(
      pdPASS == xTaskCreate(ArdOs_genericRun, name, stack, reinterpret_cast<void*>(&params[nextThreadRank]), priority, threads[nextThreadRank]),
      "Task creation failed.");

  //increment the table index
  ++nextThreadRank;
}

Signal
ArdOs::Signal_create ()
{
  ardAssert(state == UNINIT,
	    "ArdOs is not in the right state to do a signal creation");
  Signal s = xSemaphoreCreateBinary();
  ardAssert(s != NULL, "No more heap");
  ++signalCount;
  return s;
}

void
ArdOs::Signal_set (Signal s)
{
  ardAssert(state == RUNNING,
	    "ArdOs is not in the right state to set a signal.");
  xSemaphoreGive(s);
}

//void
//Signal::setFromIsr ()
//{
//  ardAssert(sem != 0, "Signal not initialized.");
//  portBASE_TYPE xHigherPriorityTaskWoken = 0;
//  xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
//}

void
ArdOs::Signal_wait (Signal s)
{
  ardAssert(state == RUNNING,
	    "ArdOs is not in the right state to wait for a signal.");
  xSemaphoreTake(s, portMAX_DELAY);
}

Mutex
ArdOs::Mutex_create ()
{
  ardAssert(state == UNINIT,
	    "ArdOs is not in the right state to do a mutex creation");
  Mutex m = xSemaphoreCreateMutex();
  ardAssert(m != NULL, "No more heap");
  ++mutexCount;
  return m;
}

void
ArdOs::Mutex_lock (Mutex s)
{
  ardAssert(state == RUNNING, "ArdOs is not in the right state to lock a mutex");
  xSemaphoreGive(s);
}

void
ArdOs::Mutex_unlock (Mutex s)
{
  ardAssert(state == RUNNING,
	    "ArdOs is not in the right state to unlock a mutex");
  xSemaphoreTake(s, portMAX_DELAY);
}

Semaphore
ArdOs::Semaphore_create (const UBaseType_t maxCount,
			 const UBaseType_t initCount)
{
  ardAssert(state == UNINIT,
	    "ArdOs is not in the right state to do a semaphore creation");
  Semaphore s = xSemaphoreCreateCounting(maxCount, initCount);
  ardAssert(s != NULL, "No more heap");
  ++signalCount;
  return s;
}

void
ArdOs::Semaphore_give (Semaphore s)
{
  ardAssert(state == RUNNING,
	    "ArdOs is not in the right state to give a semaphore");
  xSemaphoreGive(s);
}

void
ArdOs::Semaphore_take (Semaphore s)
{
  ardAssert(state == RUNNING,
	    "ArdOs is not in the right state to take a semaphore");
  xSemaphoreTake(s, portMAX_DELAY);
}

bool
ArdOs::Semaphore_tryTake (Semaphore s)
{
  ardAssert(state == RUNNING,
	    "ArdOs is not in the right state to take a semaphore");
  return pdTRUE == xSemaphoreTake(s, 0);
}

