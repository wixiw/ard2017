/*
 * ArdOs.cpp
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#include "ArdOs.h"
#include "BSP.h"

using namespace ard;

#define HEARTBEAT_PIN LED_DUE_RX

//-------------------------------------------------------------------------------

//infinite wait signal
Signal infinite;

void ArdOs::die()
{
    Signal_wait(infinite);
}

void enterIdleCB()
{
    digitalWrite(HEARTBEAT_PIN, LOW);
}

void exitIdleCB()
{
    digitalWrite(HEARTBEAT_PIN, HIGH);
}

//-------------------------------------------------------------------------------

SwTimer::SwTimer()
        : m_entryDate(0U), m_delay(0U), m_started(false)
{
}

void SwTimer::arm(uint32_t delayInMs)
{
    ardAssert(delayInMs != 0, "Delay shall be non null.");
    m_entryDate = millis();
    m_delay = delayInMs;
    m_started = true;
}

void SwTimer::cancel()
{
    m_started = false;
    m_delay = 0U;
    m_entryDate = 0U;
}

bool SwTimer::isFired() const
{
    if (m_started && m_delay <= millis() - m_entryDate)
        return true;
    else
        return false;
}

//-------------------------------------------------------------------------------

//singleton instanciation
ArdOs ArdOs::instance = ArdOs();

//helper to prevent user from exiting their threads, as it push FreeRtos in assert
void ArdOs_genericRun(void* pvParameters)
{
    auto params = reinterpret_cast<ArdOs::genericRunParams*>(pvParameters);
    ardAssert(params != NULL, "Generic params cast failed.");

    //Informs that the task is started
    char taskName[configMAX_TASK_NAME_LEN];
    ard_getTaskName(taskName);
    g_ArdOs.dprintln (String ("[ArdOs] ") + taskName + " is running.");

    //The thread is periodic
    if (params->period)
    {
        auto lastWakeTime = xTaskGetTickCount();
        while (2)/* because 1 is has-been*/
        {
            //Run either the function or the class run method
            if (params->pClass)
                params->pClass->run();
            else
                params->method();

            //wait until next period
            vTaskDelayUntil(&lastWakeTime, params->period);
        }
    }
    //The thread isnot periodic
    else
    {
        //Run either the function or the class run method
        if (params->pClass)
            params->pClass->run();
        else
            params->method();

        //Wait infinitly so that the thread context is never exited (else FreeRtos would asserts)
        g_ArdOs.dprintln (String ("[ArdOs] ") + taskName + " is finished.");
        g_ArdOs.Signal_wait(infinite);
    }

}

ArdOs::ArdOs()
{
    nextThreadRank = 0;
    heartbeatCounter = 0;
    heartbeatPinValue = 0;
    signalCount = 0;
    mutexCount = 0;
    state = eOsState::UNINIT;
    bootDuration = 0;
    debugSerialMutex = NULL;
    INIT_TABLE_TO_ZERO(threads);
    INIT_TABLE_TO_ZERO(params);
}

void ArdOs::init()
{
    ardAssert(state == eOsState::UNINIT, "ArdOs is not in the right state to do an init");
    infinite = Signal_create();
    debugSerialMutex = Mutex_create();

    digitalWrite(LED_DUE_L, LOW);
    digitalWrite(HEARTBEAT_PIN, heartbeatPinValue);

    // start FreeRTOS
    state = eOsState::RUNNING;
    bootDuration = millis();
    dprintln(String("[ArdOs] ") + "Robot is booted successfully, it took " + bootDuration + " ms.");
    vTaskStartScheduler();

    // should never reach this as the vTaskStartScheduler never ends
    dprintln("ERROR : Scheduler exited !");
    while (1)
    {
    };
}

void ArdOs::displayStats()
{
    char text[40 * configMAX_PRIORITIES];
    vTaskList(text);
    dprintln("--------------- ArdOs Stats  ------------------");
    dprintln("|   Thread   | State | Prio | Free stack | ID |");
    dprintln("-----------------------------------------------");
    dprintln(text);
    dprintln(" * States : blocked ('B'), ready ('R'), deleted ('D') or suspended ('S').");
    dprintln(" * Priority : higher number, higher priority");
    dprintln("-----------------------------------------------");
    dprintln(String("Nb Threads : ") + String(nextThreadRank + 2) + " / " + String(configMAX_PRIORITIES + 1));
    dprintln(String("Nb Mutexes : ") + mutexCount);
    dprintln(String("Nb Signals : ") + signalCount);
    dprintln(String("Booted in ") + bootDuration + " ms.");
    dprintln("-----------------------------------------------");

    //TODO static reportStackSizes
    //TODO static reportCpuConsumption
}

void ArdOs::dprintln(String s)
{
    //it's not possible to protect the link until the OS is setup (then the mutex pointer is no more NULL
    //but when set, use the mutex.
    if (debugSerialMutex)
    {
        xSemaphoreGive(debugSerialMutex);
        Serial.println(s);
        Serial.flush();
        xSemaphoreGive(debugSerialMutex);
    }
    else
    {
        Serial.println(s);
        Serial.flush();
    }
}

void ArdOs::createThread_C(const char * const name, ThreadRunFct runFunction, uint16_t stack, uint16_t priority)
{
    ardAssert(state == eOsState::UNINIT, "ArdOs is not in the right state to do a thread creation");

    //Check inputs
    ardAssert(nextThreadRank <= configMAX_PRIORITIES - 1, "Too many threads."); //there is configMAX_PRIORITIES + 1 threads, but 2 are reserved for IDLE task, and Arduino OS task so +1-2 = -1
    ardAssert(priority <= configMAX_PRIORITIES - 1,
            String(name) + " priority ("+priority+") is too high (max is "+String(configMAX_PRIORITIES - 1)+"), check for stack/priority order or increase max value in FreeRtosConfig.h."); // configMAX_PRIORITIES is reserved for OS thread

    //fill the params
    params[nextThreadRank].pClass = NULL;
    params[nextThreadRank].method = runFunction;

    //create the thread
    ardAssert(pdPASS == xTaskCreate(ArdOs_genericRun, name, stack, reinterpret_cast<void*>(&params[nextThreadRank]), priority, &threads[nextThreadRank]),
            "Task creation failed.");

    //increment the table index
    ++nextThreadRank;
}

void ArdOs::createThread_Cpp(const char * const name, IThread& pClass, uint16_t stack, uint16_t priority)
{
    createPeriodicThread_Cpp(name, pClass, stack, priority, 0);
}

void ArdOs::createPeriodicThread_Cpp(const char * const name, IThread& pClass, uint16_t stack, uint16_t priority, uint16_t periodMs)
{
    ardAssert(state == eOsState::UNINIT, "ArdOs is not in the right state to do a thread creation");

    //Check inputs
    ardAssert(nextThreadRank <= configMAX_PRIORITIES - 1, "Too many threads."); //there is configMAX_PRIORITIES + 1 threads, but 2 are reserved for IDLE task, and Arduino OS task so +1-2 = -1
    ardAssert(priority <= configMAX_PRIORITIES - 1,
            String(name) + " priority ("+priority+") is too high (max is "+String(configMAX_PRIORITIES - 1)+"), check for stack/priority order or increase max value in FreeRtosConfig.h."); // configMAX_PRIORITIES is reserved for OS thread

    //fill the params
    params[nextThreadRank].pClass = &pClass;
    params[nextThreadRank].method = NULL;
    params[nextThreadRank].period = periodMs;

    //create the thread
    ardAssert(pdPASS == xTaskCreate(ArdOs_genericRun, name, stack, &params[nextThreadRank], priority, &threads[nextThreadRank]),
            "Task creation failed.");

    //increment the table index
    ++nextThreadRank;
}

Signal ArdOs::Signal_create()
{
    ardAssert(state == eOsState::UNINIT, "ArdOs is not in the right state to do a signal creation");
    auto s = xSemaphoreCreateBinary();
    ardAssert(s != NULL, "No more heap");
    ++signalCount;
    return s;
}

void ArdOs::Signal_set(Signal s)
{
    ardAssert(state == eOsState::RUNNING, "ArdOs is not in the right state to set a signal.");
    xSemaphoreGive(s);
}

void ArdOs::Signal_setFromIsr(Signal s)
{
    ardAssert(state == eOsState::RUNNING, "ArdOs is not in the right state to set a signal.");
    portBASE_TYPE xHigherPriorityTaskWoken = 0;
    xSemaphoreGiveFromISR(s, &xHigherPriorityTaskWoken);
}

void ArdOs::Signal_wait(Signal s)
{
    ardAssert(state == eOsState::RUNNING, "ArdOs is not in the right state to wait for a signal.");
    xSemaphoreTake(s, portMAX_DELAY);
}

Mutex ArdOs::Mutex_create()
{
    ardAssert(state == eOsState::UNINIT, "ArdOs is not in the right state to do a mutex creation");
    auto m = xSemaphoreCreateMutex();
    ardAssert(m != NULL, "No more heap");
    ++mutexCount;
    return m;
}

void ArdOs::Mutex_lock(Mutex s)
{
    ardAssert(state == eOsState::RUNNING, "ArdOs is not in the right state to lock a mutex");
    xSemaphoreTake(s, portMAX_DELAY);
}

void ArdOs::Mutex_unlock(Mutex s)
{
    ardAssert(state == eOsState::RUNNING, "ArdOs is not in the right state to unlock a mutex");
    xSemaphoreGive(s);
}

Semaphore ArdOs::Semaphore_create(const UBaseType_t maxCount, const UBaseType_t initCount)
{
    ardAssert(state == eOsState::UNINIT, "ArdOs is not in the right state to do a semaphore creation");
    auto s = xSemaphoreCreateCounting(maxCount, initCount);
    ardAssert(s != NULL, "No more heap");
    ++signalCount;
    return s;
}

void ArdOs::Semaphore_give(Semaphore s)
{
    ardAssert(state == eOsState::RUNNING, "ArdOs is not in the right state to give a semaphore");
    xSemaphoreGive(s);
}

void ArdOs::Semaphore_take(Semaphore s)
{
    ardAssert(state == eOsState::RUNNING, "ArdOs is not in the right state to take a semaphore");
    xSemaphoreTake(s, portMAX_DELAY);
}

bool ArdOs::Semaphore_tryTake(Semaphore s)
{
    ardAssert(state == eOsState::RUNNING, "ArdOs is not in the right state to take a semaphore");
    return pdTRUE == xSemaphoreTake(s, 0);
}

void ArdOs::sleep_ms(uint16_t durationMs)
{
    vTaskDelay(durationMs);
}

//-------------------------------------------------------------------------------

IEvent* EventListener::waitEvents(IEvent* listenedEvts[], int nbEvents)
{
    ardAssert(listenedEvts != NULL, "EventListener::waitEvents : null pointer to table.");
    
    //subsribe to the event list
    for( int i = 0 ; i < nbEvents ; ++i )
    {
        ardAssert(listenedEvts[i] != NULL, "EventListener::waitEvents : null pointer to event.");
        listenedEvts[i]->subscribe(this);
    }

    IEvent* receivedEvent = NULL;
    //block until one event is emitted
    if (!xQueueReceive(queue, &receivedEvent, portMAX_DELAY))
    {
        ardAssert(false, "EventListener::wait : unexpected return code");
    }

    //unsubsribe to the event list
    for( int i = 0 ; i < nbEvents ; ++i )
    {
        listenedEvts[i]->unsubscribe(this);
    }

    return receivedEvent;
}

void EventListener::privateSend(IEvent* publisher)
{
    ardAssert(publisher != NULL, "EventListener::privateSend don't expect an invalid event.");
    if (!xQueueSendToBack(queue, &publisher, 0))
    {
        ardAssert(false, "EventListener::privateSend : queue is full");
    }
}

void EventListener::privateSendFromISR(IEvent* publisher)
{
    configASSERT(publisher != NULL); //ardAssert is not accessible from interrupt
    if (!xQueueSendToBackFromISR(queue, &publisher, 0))
    {
        configASSERT(false);//ardAssert is not accessible from interrupt
    }
}
