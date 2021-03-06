/*
 * ArdOs.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ARDOS_H_
#define ROBOTS_ARDOS_H_

#include "core/ArdFramework.h"
#include "K_thread_config.h"
#include "FreeRtos/FreeRTOS_ARM.h"
#include "ArduinoCore/Arduino.h"

namespace ard
{
    //-------------------------------------------------------------------------------
    //                      OsObject
    //-------------------------------------------------------------------------------

    /**
     * Abstract class to gather all objets represeting an OS concept
     * Usefull to get statistics and ensure OS lifecycle is correct
     */
    class OsObject: public ArdObject
    {
    public:
        OsObject(String const& name = String::empty);

        //will assert, it is forbidden to destroy an OS object
        virtual ~OsObject();

        //Test if object is executed from interrupt mode true if code is called from interrupt
        static bool interruptContext()
        {
            return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0 ;
        }
    };


    //-------------------------------------------------------------------------------
    //                      SwTimer
    //-------------------------------------------------------------------------------

    /**
     * Use this class to create timeout or fire an action after a delay
     * It's a passive class, based on system clock memorisation
     * The comparaison is done when isFired() is called
     */
    class SwTimer: public OsObject
    {
    public:
        SwTimer();

        //Start the timer and load a delay in milliseconds
        void
        arm(DelayMs delayInMs);

        //Cancel the timer execution
        void
        cancel();

        //Test is the timer is fired
        bool
        isFired() const;

        //static getter for statistics
        static uint8_t getOsObjectCount() //const : no-const on a static member
        {
            return objectCount;
        }

    private:
        TimeMs m_entryDate;
        DelayMs m_delay;
        bool m_started;

        //count how many object of that type has been initialized
        //note that uninitialized object are not considered as they are not used
        static uint8_t objectCount;
    };


    //-------------------------------------------------------------------------------
    //                      Mutex
    //-------------------------------------------------------------------------------

    /**
     * Use Mutex to protect shared memories in multi threaded environnements.
     * Do not use this for thread synchronization, prefer a Signal for that use.
     * Do not use from interrupt context, it's a no-sense, prefer a critical section for that
     */
    class Mutex: public OsObject
    {
    public:
        Mutex();

        //Implements OsObject : creates the OS object
        //User should not call this, it is automatically called during the ArdOs::init() call
        void init() override;

        //Wait for Mutex to be free, and get exclusivity
        //lock may by exited sooner if a timeout is set
        //using a 0 value is equivalent to a "tryLock()"
        void lock(DelayMs timeout = portMAX_DELAY);

        //Release Mutex exclusivity
        void unlock();

        //static getter for statistics
        static uint8_t getOsObjectCount() //const : no-const on a static member
        {
            return objectCount;
        }

    private:
        SemaphoreHandle_t osHandler;

        //count how many object of that type has been initialized
        //note that uninitialized object are not considered as they are not used
        static uint8_t objectCount;

    };

    class FakeMutex
    {
    public:
        void lock(DelayMs timeout = portMAX_DELAY){};
        void unlock(){};
    };

    //-------------------------------------------------------------------------------
    //                      Signal
    //-------------------------------------------------------------------------------
    /**
     * Use Signal to syncronize threads in multi threaded environnements.
     * Do not use this for memory protection, prefer a Mutex for that use.
     */
    class Signal: public OsObject
    {
    public:
        Signal();

        //Implements OsObject : creates the OS object
        //User should not call this, it is automatically called during the ArdOs::init() call
        void init() override;

        //Wait for the Signal to bet set()
        //or that the timeout elasped
        //using a 0 value is equivalent to a "tryLock()"
        void wait(DelayMs timeout = portMAX_DELAY);

        //Send the signal so that a wait()'ing thread is unblocked.
        //If several thread are waiting, the next set will only awake one waiter
        void set();

        //static getter for statistics
        static uint8_t getOsObjectCount() //const : no-const on a static member
        {
            return objectCount;
        }

    private:
        SemaphoreHandle_t osHandler;

        //count how many object of that type has been initialized
        //note that uninitialized object are not considered as they are not used
        static uint8_t objectCount;
    };

    //-------------------------------------------------------------------------------
    //                      Semaphore
    //-------------------------------------------------------------------------------
    /**
     * Use Semaphore to share a ressource count.
     * Do not use this for memory protection, use a Mutex aside.
     * Note that a semaphore is not holding data, it just reference
     * the number a free/taken ressources
     */
    class Semaphore: public OsObject
    {
    public:
        Semaphore(uint16_t maxCount, uint16_t startingCount);

        //Implements OsObject : creates the OS object
        //User should not call this, it is automatically called during the ArdOs::init() call
        void init() override;

        //Take a new resource from the reserve or block until one is available
        //A timeout of 0 is equivalent of tryTake()
        void take(DelayMs timeout = portMAX_DELAY);

        //same as take(), but do not block when the reserve is empty, return false instead
        //USE ONLY IN INTERRUPT CONTEXT
        bool takeFromISR();

        //Put a new ressource in the reserve or block if no room is available
        //A timeout of 0 is equivalent of tryGive()
        //DO NOT USE FROM INTERRUPT CONTEXT
        void give(DelayMs timeout = portMAX_DELAY);

        //same as give(), but do not block when the reserve is full, return false instead
        //USE ONLY IN INTERRUPT CONTEXT
        bool giveFromISR();

        //retrieve the number of item in the semaphore
        //just for information or debug, you should not require to call this
        uint32_t getCount();

        //static getter for statistics
        static uint8_t getOsObjectCount() //const : no-const on a static member
        {
            return objectCount;
        }

    private:
        SemaphoreHandle_t osHandler;

        //count how many object of that type has been initialized
        //note that uninitialized object are not considered as they are not used
        static uint8_t objectCount;

        uint16_t maxCount;
        uint16_t startingCount;
    };


    //-------------------------------------------------------------------------------
    //                      Queue
    //-------------------------------------------------------------------------------
    /**
     * Use a queue to send date to a thread
     * Do not use to synchronize thread, prefer a Signal.
     * In fact it's the opposite a queue is often used to
     * decouple 2 thread timings constraints.
     */
    class Queue: public OsObject
    {
    public:
        /**
         * @param nbItems : the maximal number of items that can wait in the queue
         * @param itemSize : the size of the biggest object to put in the queue
         */
        Queue(uint8_t nbItems, size_t itemSize);

        //Implements OsObject : creates the OS object
        void init() override;

        //Push an object into the queue
        //this call may be blocking if no room is available
        //the call will not block more than timeout
        //using a 0 value is equivalent to a "tryLock()"
        //@return false if the queue is full, true on success
        bool push(void const * const queuedObject, DelayMs timeout = portMAX_DELAY);

        //using a 0 value is equivalent to a "tryLock()"
        //may be used either from ISR or from thread
        //@return false if the queue is empty, true on success
        bool pop(void* unqueuedObject, DelayMs timeout = portMAX_DELAY);

        //empty queue
        void empty();

        //Return the number of objects in the queue
        uint8_t getAvailableSpace();

        //For statistics : the maximal number of items it has been in the queue.
        uint8_t getMinAvailSpace()
        {
            return queueMinAvailSpace;
        }

        //static getter for statistics
        static uint8_t getOsObjectCount() //const : no-const on a static member
        {
            return objectCount;
        }

    private:
        QueueHandle_t osHandler;
        uint8_t nbItems;
        size_t itemSize;

        //For statistics : the minimal number of free slot during queue life.
        uint8_t queueMinAvailSpace;

        //count how many object of that type has been initialized
        //note that uninitialized object are not considered as they are not used
        static uint8_t objectCount;
    };

    //-------------------------------------------------------------------------------
    //                      Thread
    //-------------------------------------------------------------------------------

    /**
     * Inherit this abstract in order to create a Thread
     * from a C++ class.
     *
     * Note that the thread is automatically waiting on a start signal you have to provide
     * with start();
     */
    class Thread: public OsObject
    {
        friend void Thread_genericRun(void* pvParameters);

    public:
        //struct to gather parameters to pass to ArdOs_genericRun
        class ThreadParams
        {
        public:

            bool            used;
            TaskHandle_t    handle;
            Thread*         object;
            StackSize       stackSize;
            DelayMs         period;

            ThreadParams():
                used(false),
                handle(NULL),
                object(NULL),
                stackSize(0),
                period(0){}
        };

        /**
         * @param name : a name for debug/introspection purposes, length is limited by configMAX_TASK_NAME_LEN
         * @param priority : the Thread priority, it shall not be equal to another existing one, the lowest value, the lowest priority
         * @param stackSize : the stack length in WORDS (not bytes), hence 100 = 400o
         * @param period (optional) : the period at which run() is called. run is called once is not defined or set to 0
         */
        Thread(String const& name,
                ThreadPriority priority,
                StackSize stackSize,
                DelayMs period=0);

        //Implements OsObject : creates the thread OS object
        //User should not call this, it is automatically called during the ArdOs::init() call
        virtual void init() override;

        //Start the thread
        virtual void startThread();

        //Stop the thread
        virtual void stopThread();

        //Implement this function as the Thread main function
        virtual void run() = 0;

        //We may optionally connect a logger to all threads
        static void setLogger(ILogger* newLogger);

        //disable OS interruptions, this call is protected against nested calls
        static void enterCriticalSection()
        {
            taskENTER_CRITICAL();
        }

        //enable OS interruption, this call is protected against nested calls
        static void exitCriticalSection()
        {
            taskEXIT_CRITICAL();
        }

        //Implements ILogger interface
        void logFromThread(eLogLevel lvl, String const& text);

        //Start triggering gpio for oscilloscope analysis
        //It generates a PWM with high state being the computation time
        //@param pin : Arduino pin ID
        void activateDebug(uint8_t pin)
        {
            debugPin = pin;
        }

        //getter
        StackSize getStackSize() const;

        //getter
        DelayMs getPeriod() const;

        //inline getter
        ThreadPriority const& getPriority() const
        {
            return priority;
        }

        //static getter for statistics
        static uint8_t getOsObjectCount() //const : no-const on a static member
        {
            return objectCount;
        }

        //@return: a table of Thread::ThreadParams[PRIO_NB] containing all threads data
        static ThreadParams const * getThreadParams();

        //a signal that is never set to block a thread
        static Signal* infinite;

    protected:
        //table of all params so that the generic run function is possible
        //it is also used to test if a priority is already in use
        //it's placed here to ensure proper/controller initialization of memory (by opposition of a global/static var)
        //it will be initialized by first constructor as a threadParams[PRIO_NB];
        static ThreadParams* threadParams;

        //Put thread in sleep mode during delay ms.
        void sleepMs(DelayMs delay);

        //Call this in your run() function to trigger the debug gpios : signals the start of the period
        //inlined to prevent stack pile overflow
        void debugTrace_beginLoop() const
        {
            if(debugPin)
            {
                digitalWrite(debugPin, HIGH);
            }
        }

        //Call this in your run() function to trigger the debug gpios : signals the end of the periods
        //inlined to prevent stack pile overflow
        void debugTrace_endLoop() const
        {
            if(debugPin)
            {
                digitalWrite(debugPin, HIGH);
            }
        }

    private:
        //see constructor
        ThreadPriority priority;

        //The pin on which to send debug signals
        uint8_t debugPin;

        //the one that can log for us
        static ILogger* logger;

        //count how many object of that type has been initialized
        //note that uninitialized object are not considered as they are not used
        static uint8_t objectCount;
    };

    //-------------------------------------------------------------------------------
    //                     Poller Thread
    //-------------------------------------------------------------------------------

    /**
     * Use a PolledObject when you need be updated periodically
     * without having enougth to do to justify a periodic thread creation
     */
    class PolledObject: public OsObject
    {
    public:
        PolledObject(String const& name = String::empty):OsObject(name){};

        //polling event
        virtual void update(DelayMs sinceLastCall) = 0;
    };

    class PollerThread: public Thread
    {
    public:
        PollerThread(String const& name,
                ThreadPriority priority,
                StackSize stackSize,
                DelayMs period,
                uint8_t nbPolledObjects);

        //Overrides Thread : initialize all polled objects
        //User should not call this, it is automatically called during the ArdOs::init() call
        virtual void init() override;

        //Implements Threads : run all actuator systems and sensors
        virtual void run () override;

        //Add a polled object the list, shall be called before calling init()
        void addPolledObject(PolledObject& object );

    private:
        PolledObject** polledObjects;
        uint8_t nextRank;
        uint8_t nbMaxObjects;
    };


//-------------------------------------------------------------------------------
//                      ArdOs
//-------------------------------------------------------------------------------

    /**
     * Manage instanciation of OS Objects
     * This class is not thread safe, initialization is supposed to
     * be executed from one unique thread
     *
     * It's a pure static class
     */
    class ArdOs
    {
    public:
        typedef enum eOsState
        {
            INITIALIZING, READY, RUNNING
        } eOsState;

        //initialize all OS objects
        //User should not call this, it is automatically called during the ArdOs::init() call
        static void init();

        //enable interrupts and start the scheduler, init() must have been called once before
        //this function never returns in normal conditions
        static void start();

        //Stop all threads and disable interrupts
        static void stop();

        //add a new OS object to the list
        static void registerObject(OsObject* object);

        //Makes the calling thread to sleep for a while
        static void sleepMs(DelayMs delay);

        //SW reset
        static void reboot()
        {
            NVIC_SystemReset();
        }

        //disable interruption (all interruptions, even the one that are not managed by FreeRtos)
        //this call is NOT protected against nested calls, so be sure that no one re-enable interrupts
        static void disableAllInterrupts()
        {
            __disable_irq();
        }

        //enable interruption (all interruptions, even the one that are not managed by FreeRtos)
        //this call is NOT protected against nested calls, so be sure that you are not fooling someone by
        //re-enabling interrupts in his back
        static void enableAllInterrupts()
        {
            __enable_irq();
        }

        //Get the OS initialization state
        static eOsState getState()
        {
            return state;
        }

        //Maximal number of Objects that can be registerObject()'ed
        static const uint8_t MAX_OBJECT_NB = 100;

    private:
        //Tracking of initialization status to prevent stupid errors
        static eOsState state;

        //counts the total number of objects registered with registerObject
        static uint8_t objectCount;

        //private constructor as its a singleton class
        ArdOs();
        COPY_CONSTRUCTORS (ArdOs)
    };

}	//end namespace

#endif /* ROBOTS_ARDOS_H_ */
