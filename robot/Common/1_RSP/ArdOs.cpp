/*
 * ArdOs.cpp
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#include "ArdOs.h"
#include "BSP.h"

using namespace ard;

#define ASSERT_OS_STARTED ASSERT_TEXT(ArdOs::getState() == ArdOs::RUNNING, "OS must be started before using this function.")

//-------------------------------------------------------------------------------
//                      OsObject
//-------------------------------------------------------------------------------
OsObject::OsObject(String const& name):
                ArdObject(name)
{
    ArdOs::registerObject(this);
}

OsObject::~OsObject(){};


//-------------------------------------------------------------------------------
//                      SwTimer
//-------------------------------------------------------------------------------

//static member instanciation
uint8_t SwTimer::objectCount = 0;

SwTimer::SwTimer()
: OsObject(),
  m_entryDate(0U),
  m_delay(0U),
  m_started(false)
{
    ASSERT_TEXT(objectCount < 0xFF , "Too many objects of that type.");
    objectCount++;
}

void SwTimer::arm(uint32_t delayInMs)
{
    ASSERT(isInitialized());
    ASSERT_OS_STARTED;
    ASSERT_TEXT(delayInMs != 0, "Delay shall be non null.");
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
    ASSERT(isInitialized());
    ASSERT_OS_STARTED;
    if (m_started && m_delay <= millis() - m_entryDate)
        return true;
    else
        return false;
}

//-------------------------------------------------------------------------------
//                      Mutex
//-------------------------------------------------------------------------------

//static member instanciation
uint8_t Mutex::objectCount = 0;

Mutex::Mutex():
                OsObject(),
                osHandler(NULL)
{
    ASSERT_TEXT(objectCount < 0xFF , "Too many objects of that type.");
    objectCount++;
}

void Mutex::init()
{
    OsObject::init();
    osHandler = xSemaphoreCreateMutex();
}

void Mutex::lock(DelayMs timeout)
{
    ASSERT(isInitialized());
    ASSERT_TEXT(!interruptContext(),"a mutex cannot be used with interrupts, use critical section instead.");

    BaseType_t res = pdFALSE;
    do
    {
        res = xSemaphoreTake(osHandler, timeout);
    }
    //retry if delay is set to maximum value
    while(timeout == portMAX_DELAY && res == pdFALSE );

}

void Mutex::unlock()
{
    ASSERT(isInitialized());
    ASSERT_TEXT(!interruptContext(),"a mutex cannot be used with interrupts, use critical section instead.");
    xSemaphoreGive(osHandler);
}

//-------------------------------------------------------------------------------
//                      Signal
//-------------------------------------------------------------------------------

//static member instanciation
uint8_t Signal::objectCount = 0;

Signal::Signal():
        OsObject(),
        osHandler(NULL)
{
    ASSERT_TEXT(objectCount < 0xFF , "Too many objects of that type.");
    objectCount++;
}

void Signal::init()
{
    OsObject::init();
    osHandler = xSemaphoreCreateBinary();
}

void Signal::wait(DelayMs timeout)
{
    ASSERT(isInitialized());
    ASSERT_TEXT(!interruptContext(), "You can't wait for a signal inside an interrupt");

    BaseType_t res = pdFALSE;
    do
    {
        res = xSemaphoreTake(osHandler, timeout);
    }
    //retry if delay is set to maximum value
    while(timeout == portMAX_DELAY && res == pdFALSE );
}

void Signal::set()
{
    ASSERT(isInitialized());
    if(interruptContext())
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(osHandler, &xHigherPriorityTaskWoken);
        //force context switch if a task with higher priority is awoken
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else
        xSemaphoreGive(osHandler);
}

//-------------------------------------------------------------------------------
//                      Queue
//-------------------------------------------------------------------------------

//static member instanciation
uint8_t Queue::objectCount = 0;

Queue::Queue(uint8_t nbItems, size_t itemSize):
    OsObject(),
    osHandler(NULL),
    nbItems(nbItems),
    itemSize(itemSize),
    queueMinAvailSpace(0)
{
    ASSERT_TEXT(objectCount < 0xFF , "Too many objects of that type.");
    objectCount++;
}

void Queue::init()
{
    OsObject::init();
    osHandler = xQueueCreate(nbItems, itemSize);
}

bool Queue::push(void* queuedObject, DelayMs timeout)
{
    ASSERT(isInitialized());
    BaseType_t res = pdFALSE;

    if( interruptContext() )
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        res = xQueueSendFromISR(osHandler, queuedObject, &xHigherPriorityTaskWoken);
        //force context switch if a task with higher priority is awoken
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        do
        {
            res = xQueueSend(osHandler, queuedObject, timeout);
        }
        //retry if delay is set to maximum value
        while(timeout == portMAX_DELAY && res == pdFALSE );
    }

    //Update statistics
    UBaseType_t freeSlots = getAvailableSpace();
    if( freeSlots < queueMinAvailSpace )
        queueMinAvailSpace = freeSlots;

    return res == pdTRUE;
}

bool Queue::pop(void* unqueuedObject, DelayMs timeout)
{
    ASSERT(isInitialized());
    ASSERT_TEXT(!interruptContext(), "A queue pop() shall not be used from ISR");

    BaseType_t res = pdFALSE;
    do
    {
        res = xQueueReceive(osHandler, unqueuedObject, timeout);
    }
    //retry if delay is set to maximum value
    while(timeout == portMAX_DELAY && res == pdFALSE );

    return res == pdTRUE;
}

uint8_t Queue::getAvailableSpace()
{
    return uxQueueSpacesAvailable(osHandler);
}


//-------------------------------------------------------------------------------
//                      Thread
//-------------------------------------------------------------------------------
//static member instanciation
uint8_t Thread::objectCount = 1; //IDLE task is created by the OS
ILogger* Thread::logger = NULL;
Thread::ThreadParams* Thread::threadParams = NULL;
Signal* Thread::infinite = NULL;

//FreeRtos need a "C function" whereas we have C++ methods, due to demangling issues
//we have to use a plain "C function" that wrap to the Thread class.
//helper to prevent user from exiting their threads, as it push FreeRtos in assert
void Thread_genericRun(void* pvParameters)
{
    auto params = reinterpret_cast<Thread::ThreadParams*>(pvParameters);
    ASSERT_TEXT(params != NULL, "params cast failed.");
    ASSERT(params->object != NULL);

    //The thread is periodic
    if (params->period)
    {
        params->object->logFromThread(eLogLevel_DEBUG, String("Periodic thread started (stack=")
                + params->object->getStackSize()
                + "w prio=" + params->object->getPriority()
                + " p=" + params->object->getPeriod() + "ms)");

        auto lastWakeTime = xTaskGetTickCount();
        while (2)/* because 1 is has-been*/
        {
            //execute the class run method
            params->object->run();

            //wait until next period
            vTaskDelayUntil(&lastWakeTime, params->period);
        }
    }
    //The thread is not periodic
    else
    {
        params->object->logFromThread(eLogLevel_DEBUG, String("Aperiodic thread started (stack=")
                + params->object->getStackSize()
                + "w prio=" + params->object->getPriority()
                + ")");

        //execute the class run method
        params->object->run();

        //Wait infinitly so that the thread context is never exited (else FreeRtos would asserts)
        params->object->logFromThread(eLogLevel_INFO, "Thread exited due to run() execution end.");
        Thread::infinite->wait();
    }
}

Thread::Thread(String const& name, ThreadPriority priority, StackSize stackSize, DelayMs period):
                OsObject(name),
                priority(priority),
                debugPin(0)
{
    ASSERT_TEXT(objectCount < 0xFF , "Too many objects of that type.");
    objectCount++;

    //initialize the thread param table the first time
    //it's done here in place of a global/static var for memory initialization race conditions
    if( threadParams == NULL )
    {
        threadParams = (ThreadParams*) malloc(sizeof(ThreadParams)*PRIO_NB);

        //FreeRtos will manage the idle thread for us, for clarity
        //an entry is registered here
        threadParams[0].used         = true;
        threadParams[0].handle       = NULL; //Idle task handler is initalized later by vTaskStartScheduler()
        threadParams[0].object       = NULL;
        threadParams[0].stackSize    = configMINIMAL_STACK_SIZE;
        threadParams[0].period       = 0;

        infinite = new Signal();
    }

    threadParams[priority].used         = false;
    threadParams[priority].handle       = NULL;
    threadParams[priority].object       = NULL;
    threadParams[priority].stackSize    = stackSize;
    threadParams[priority].period       = period;
}

void Thread::init()
{
    ASSERT(getName().length() <= configMAX_TASK_NAME_LEN);

    //refining parent function requires to call parent one before
    OsObject::init();

    //Check inputs
    ASSERT_TEXT(!threadParams[priority].used, "a Thread already exist at this priority");
    ASSERT_TEXT(priority <= PRIO_MAX, "priority is too high");
    threadParams[priority].used         = true;
    threadParams[priority].object       = this;

    //create the OS thread and initialize the Thread handler
    BaseType_t res = xTaskCreate(
            Thread_genericRun,
            getName().c_str(),
            threadParams[priority].stackSize,
            &(threadParams[priority]),
            priority,
            &(threadParams[priority].handle));
    ASSERT_TEXT(pdPASS == res, "task creation failed");
}


void Thread::startThread()
{
    ASSERT_TEXT(isInitialized(), "not initialized");
    vTaskResume(threadParams[priority].handle);
}

void Thread::stopThread()
{
    ASSERT_TEXT(isInitialized(), "not initialized");
    //Request thread to stay blocked until decided
    //TODO vTaskSuspend(threadParams[priority].handle);
    infinite->wait();
}

void Thread::setLogger(ILogger* newLogger)
{
    ASSERT_TEXT( newLogger != NULL, "you tried to configure a NULL logger.");
    ASSERT_TEXT( logger == NULL,    "you tried to configure a logger twice.");
    logger = newLogger;
}

void Thread::logFromThread(eLogLevel lvl, String const& text)
{
    if(logger)
    {
        logger->log(lvl, text);
    }
}

StackSize Thread::getStackSize() const
{
    return threadParams[priority].stackSize;
}

DelayMs Thread::getPeriod() const
{
    return threadParams[priority].period;
}

Thread::ThreadParams const * Thread::getThreadParams()
{
    return threadParams;
}

void Thread::sleepMs(DelayMs delay)
{
    ArdOs::sleepMs(delay);
}

//-------------------------------------------------------------------------------
//                     Poller Thread
//-------------------------------------------------------------------------------

PollerThread::PollerThread(String const& name,
        ThreadPriority priority,
        StackSize stackSize,
        DelayMs period,
        uint8_t nbPolledObjects):
                Thread(name, priority, stackSize, period),
                polledObjects(NULL),
                nextRank(0),
                nbMaxObjects(nbPolledObjects)
{
    polledObjects = (PolledObject**)malloc(sizeof(PolledObject*));
}

void PollerThread::init()
{
    Thread::init();

    for(int i = 0 ; i < nextRank ; ++i)
    {
        polledObjects[i]->init();
    }
}

void PollerThread::run()
{
    debugTrace_beginLoop();

    for(int i = 0 ; i < nextRank ; ++i)
    {
        polledObjects[i]->update(getPeriod());
    }

    debugTrace_endLoop();
}

void PollerThread::addPolledObject(PolledObject& object )
{
    ASSERT(!isInitialized());
    ASSERT_TEXT(nextRank < nbMaxObjects, "too many mini objects");
    polledObjects[nextRank] = &object;
    nextRank++;
}

//-------------------------------------------------------------------------------
//                      ArdOs
//-------------------------------------------------------------------------------

//static member instanciation
ArdOs::eOsState ArdOs::state        = ArdOs::eOsState::INITIALIZING;
uint8_t         ArdOs::objectCount  = 0;

//Table to register objects
OsObject* objectList[ArdOs::MAX_OBJECT_NB] = {NULL};

void ArdOs::init()
{
    ASSERT(state==eOsState::INITIALIZING);

    for( uint8_t i = 0; i<objectCount ; i++)
    {
        ASSERT(objectList!=NULL);
        objectList[i]->init();
    }

    state = eOsState::READY;
}

void ArdOs::start()
{
    ASSERT(state==eOsState::READY);
    state = eOsState::RUNNING;
    vTaskStartScheduler();
}

void ArdOs::stop()
{
    ASSERT(state==eOsState::RUNNING);
    state = eOsState::READY;
    vTaskSuspendAll();
}

void ArdOs::registerObject(OsObject* object)
{
    ASSERT(state==eOsState::INITIALIZING);
    ASSERT(object != NULL);
    ASSERT(objectCount < MAX_OBJECT_NB);

    //append the object to the list and increment counter
    objectList[objectCount++] = object;
}

void ArdOs::sleepMs(DelayMs delay)
{
    ASSERT(state==eOsState::RUNNING);
    vTaskDelay(delay);
}

void ArdOs::reboot()
{
    //see https://mcuoneclipse.files.wordpress.com/2015/07/aircr.png
    //or chapter 4.3.5 on "Application interrupt and reset control register
    SCB->AIRCR = (uint32_t)0x5FA << SCB_AIRCR_VECTKEY_Pos | SCB_AIRCR_SYSRESETREQ_Msk;
    while(666);
}

//-------------------------------------------------------------------------------
//                      Events
//-------------------------------------------------------------------------------
//the compiler requires that for some optimizations ...
IEventListener::~IEventListener(){}
IEvent::~IEvent(){}

IEvent* EventListener::waitEvents(IEvent* listenedEvts[], int nbEvents)
{
    ASSERT_TEXT(listenedEvts != NULL, "EventListener::waitEvents : null pointer to table.");

    //subsribe to the event list
    for( int i = 0 ; i < nbEvents ; ++i )
    {
        ASSERT_TEXT(listenedEvts[i] != NULL, "EventListener::waitEvents : null pointer to event.");
        listenedEvts[i]->subscribe(this);
    }

    IEvent* receivedEvent = NULL;
    //block until one event is emitted
    if (!xQueueReceive(queue, &receivedEvent, portMAX_DELAY))
    {
        ASSERT_TEXT(false, "EventListener::wait : unexpected return code");
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
    ASSERT_TEXT(publisher != NULL, "EventListener::privateSend don't expect an invalid event.");
    if (!xQueueSendToBack(queue, &publisher, 0))
    {
        ASSERT_TEXT(false, "EventListener::privateSend : queue is full");
    }
}

void EventListener::privateSendFromISR(IEvent* publisher)
{
    ASSERT(publisher != NULL);
    if (!xQueueSendToBackFromISR(queue, &publisher, 0))
    {
        ASSERT(false);
    }
}
