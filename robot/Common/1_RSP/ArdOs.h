/*
 * ArdOs.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ARDOS_H_
#define ROBOTS_ARDOS_H_

#include "BSP.h"
#include "core/ArdFramework.h"
#include "K_thread_config.h"

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

        //Implements ArdObject : creates the thread OS object
        virtual void init() override;

        //static getter for statistics
        static uint8_t getCount()
        {
            return objectCount;
        }

        //Test if object is executed from interrupt mode true if code is called from interrupt
        static bool interruptContext()
        {
            return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0 ;
        }

    private:
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

        //Implements ArdObject : creates the thread OS object
        virtual void init() override;

        //Start the thread
        virtual void startThread();

        //Stop the thread
        virtual void stopThread();

        //Implement this function as the Thread main function
        virtual void run() = 0;

        //We may optionally connect a logger to all threads
        static void setLogger(ILogger* newLogger);

        //getter
        StackSize getStackSize() const;

        //getter
        DelayMs getPeriod() const;

        //inline getter
        ThreadPriority const& getPriority() const
        {
            return priority;
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

    protected:
        //Put thread in sleep mode during delay ms.
        void sleepMs(DelayMs delay);

        //Call this in your run() function to trigger the debug gpios : signals the start of the period
        //inlined to prevent stack pile overflow
        void debugTrace_beginLoop()
        {
            if(debugPin)
            {
                digitalWrite(debugPin, HIGH);
            }
        }

        //Call this in your run() function to trigger the debug gpios : signals the end of the periods
        //inlined to prevent stack pile overflow
        void debugTrace_endLoop()
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
    };

    //-------------------------------------------------------------------------------
    //                     Poller Thread
    //-------------------------------------------------------------------------------

    /**
     * Use a PolledObject when you need be updated periodically
     * without having enougth to do to justify a periodic thread creation
     */
    class PolledObject: public ArdObject
    {
    public:
        //polling event
        virtual void update(TimeMs sinceLastCall) = 0;
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
        void init() override;

        //Implements Threads : run all actuator systems and sensors
        void run () override;

        //Add a polled object the list, shall be called before calling init()
        void addPolledObject(PolledObject& object );

    private:
        PolledObject** polledObjects;
        uint8_t nextRank;
        uint8_t nbMaxObjects;
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

    private:
        TimeMs m_entryDate;
        DelayMs m_delay;
        bool m_started;
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

        //Implements ArdObject : creates the OS object
        void init() override;

        //Wait for Mutex to be free, and get exclusivity
        //lock may by exited sooner if a timeout is set
        //using a 0 value is equivalent to a "tryLock()"
        void lock(DelayMs timeout = portMAX_DELAY);

        //Release Mutex exclusivity
        void unlock();

    private:
        SemaphoreHandle_t osHandler;

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

        //Implements ArdObject : creates the OS object
        void init() override;

        //Wait for the Signal to bet set()
        //or that the timeout elasped
        //using a 0 value is equivalent to a "tryLock()"
        void wait(DelayMs timeout = portMAX_DELAY);

        //Send the signal so that a wait()'ing thread is unblocked.
        //If several thread are waiting, the next set will only awake one waiter
        void set();

    private:
        SemaphoreHandle_t osHandler;
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
        Queue();

        //Implements ArdObject : creates the OS object
        void init() override;

        //Push an object into the queue
        //this call may be blocking if no room is available
        //the call will not block more than timeout
        //using a 0 value is equivalent to a "tryLock()"
        void push(DelayMs timeout = portMAX_DELAY);

        //using a 0 value is equivalent to a "tryLock()"
        void pop(DelayMs timeout = portMAX_DELAY);

    private:
        QueueHandle_t osHandler;
    };

//-------------------------------------------------------------------------------
//                      ArdOs
//-------------------------------------------------------------------------------

    /**
     * Manage instanciation of OS Objects
     * This class is not thread safe, initialization is supposed to
     * be executed from one unique thread
     * It's a pure static class
     */
    class ArdOs
    {
    public:
        typedef enum class eOsState
        {
            INITIALIZING, READY, RUNNING
        } eOsState;

        //initialize all OS objects
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

        //Get the OS initialization state
        static eOsState getState()
        {
            return state;
        }

        //Maximal number of Objects that can be registerObject()'ed
        static const uint8_t MAX_OBJECT_NB = 20;

    private:
        static eOsState state;
        static uint8_t objectCount;

        //private constructor as its a singleton class
        ArdOs();
        COPY_CONSTRUCTORS (ArdOs)
    };

//-------------------------------------------------------------------------------
//                      Events
//-------------------------------------------------------------------------------

    //forward declare as event classes are (on purpose) very coupled
    class IEvent;
    template<int nbListeners> class Event;

    //Interface used to absract the template parameter
    class IEventListener
    {
    public:
        virtual ~IEventListener();

    private:
        //private + friend classes to prevent user from using it accidentally
        friend class IEvent;
        template<int nbListeners> friend class Event;
        virtual void privateSend(IEvent* publisher) = 0;
        virtual void privateSendFromISR(IEvent* publisher) = 0;
    };

    //Interface used to abstract the template parameter
    class IEvent
    {
    public:
        virtual ~IEvent();

        //to be called by the class who wants to publish the event from a thread
        virtual void publish() = 0;

        //to be called by the class who wants to publish the event from an interrupt
        virtual void publishFromISR() = 0;

        //register to the publication
        virtual void subscribe(IEventListener* listener) = 0;

        //unregister to the publication
        virtual void unsubscribe(IEventListener* listener) = 0;
    };

    /**
     * Use this class to receive an event. An event is similar to a Signal,
     * but allows several sender/receiver to work together
     * where a signal is limited to a one-to-one connection
     * If there are too many pending event, further publisher
     * will assert
     */
    class EventListener: public IEventListener
    {
    public:
        /**
         * The method is templated by queueSize to express the number of
         * cached events.
         */
        template<int queueSize>
        void init()
        {
            //create a queue of 1 event
            queue = xQueueCreate(queueSize, sizeof(IEvent*));
        }

        //Wait for an event to come
        //@param listenedEvts : a table of events pointers
        //@param nbEvents : the number of events in the table
        IEvent* waitEvents(IEvent* listenedEvts[], int nbEvents);

    private:
        //override IEventListener
        void privateSend(IEvent* publisher) override;

        //override IEventListener
        void privateSendFromISR(IEvent* publisher) override;

        //Event mailbox
        QueueHandle_t queue;
    };

    /**
     * Use this class to emit an event. An event is similar to a Signal,
     * but allows several sender/receiver to work together
     * where a signal is limited to a one-to-one connection
     */
    template<int nbListeners> class Event: public IEvent
    {
    public:
        Event()
        {
            INIT_TABLE_TO_ZERO(listeners);
        }

        //implements IEvent
        void publish() override
                {
            for (int i = 0; i < nbListeners; ++i)
            {
                if (listeners[i] != NULL)
                    listeners[i]->privateSend(this);
            }
                }

        //implements IEvent
        void publishFromISR() override
                {
            for (int i = 0; i < nbListeners; ++i)
            {
                if (listeners[i] != NULL)
                    listeners[i]->privateSendFromISR(this);
            }
                }

        //implements IEvent
        void subscribe(IEventListener* listener) override
                {
            ASSERT_TEXT(listener, "Event::subscribe : listener should not be NULL.");
            for (int i = 0; i < nbListeners; ++i)
            {
                if (listeners[i] == NULL)
                {
                    listeners[i] = listener;
                    return;
                }
            }
            ASSERT_TEXT(false, "Event::subscribe : no more room to register a listener");
                }

        //implements IEvent
        void unsubscribe(IEventListener* listener) override
                {
            ASSERT_TEXT(listener, "Event::unsubscribe : listener should not be NULL.");
            for (int i = 0; i < nbListeners; ++i)
            {
                if (listeners[i] == listener)
                {
                    listeners[i] = NULL;
                    return;
                }
            }
            ASSERT_TEXT(false, "Event::unsubscribe : listener is not in the list");
                }

    private:
        //list of active event subscribers
        IEventListener* listeners[nbListeners];
    };

}	//end namespace

#endif /* ROBOTS_ARDOS_H_ */
