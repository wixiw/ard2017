/*
 * ArdOs.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ARDOS_H_
#define ROBOTS_ARDOS_H_

#include <stdint.h>
#include <FreeRTOS_ARM.h>

#include "ArdUtils.h"
#include "K_thread_config.h"

//comment in match, uncomment for debug
//#define ARD_DEBUG

#ifdef ARD_DEBUG
#define ardAssert(x,text) if( ( x ) == 0 ){g_ArdOs.dprintln(String("  *ASSERT* : ") + text); delay(1000);}
#else
#define ardAssert(x,text) configASSERT(x)
#endif

namespace ard
{

  typedef uint32_t TimeMs;
  typedef uint32_t DelayMs;

  /**
   * Use this interface to declare you are a thread class
   */
  class IThread
  {
  public:
    virtual void
    init () = 0;

    virtual void
    run () = 0;

    //Put thread in sleep mode during delay ms.
    void delay(DelayMs delay)
    {
      vTaskDelay(delay);
    }

    virtual ~IThread(){};
  };

  /**
   * Use IMiniThread when you aggregate several components in one periodic thread
   */
  class IMiniPeriodicThread
  {
  public:
    virtual void
    init () = 0;

    virtual void
    update (TimeMs sinceLastCall) = 0;

    virtual ~IMiniPeriodicThread(){};
  };

  /**
   * Use this class to create timeout or fire an action after a delay
   * It's a passive class, based on system clock memorisation
   * The comparaison is done when isFired() is called
   */
  class SwTimer
  {
    ;
  public:
    SwTimer ();

    //Start the timer and load a delay in milliseconds
    void
    arm (DelayMs delayInMs);

    //Cancel the timer execution
    void
    cancel ();

    //Test is the timer is fired
    bool
    isFired () const;

  private:
    TimeMs m_entryDate;
    DelayMs m_delay;
    bool m_started;
  };

  //FreeRtos is usgin the same type for all the following concepts, so we redefine them :
  typedef SemaphoreHandle_t Signal;
  typedef SemaphoreHandle_t Semaphore;
  typedef SemaphoreHandle_t Mutex;

  //alias to get ArdOs singleton instance
#define g_ArdOs ArdOs::getInstance()
  /**
   * Manage all threads and scheduler so that :
   * - all created threads are referenced so that statistics are available
   * - provide an heartbeat system on led RX (pin 72) : lowest priority task increment a counter and toggle the led,
   *   highest priority task checks the counter is incremented
   * This class is NOT thread safe, so you are supposed to access it in only one thread
   * Its static class (=static singleton).
   */
  class ArdOs
  {
  public:
    typedef void
    (*ThreadRunFct) (void);

    //struct to gather parameters to pass to ArdOs_genericRun
    typedef struct
    {
      IThread* pClass;
      ThreadRunFct method;
      uint16_t period;
    } genericRunParams;

    typedef enum class eOsState
    {
      UNINIT, INITIALIZED, RUNNING
    } eOsState;

    //retrieve the singleton instance (you should prefer the use of the g_ArdOs maccro)
    static ArdOs&
    getInstance ()
    {
      return instance;
    }
    ;

    //to be called before any action on this class
    void
    init ();

    //start the scheduler, call this after having build your application object instances
    void
    start ();

    //signal the SW is alive. Call this in the lowest priority task (loop())
    void
    kickHeartbeat ();

    //Get the OS initialization state
    eOsState
    getState () const;

    //Debug only : send statistics to the debug serial link
    void
    displayStats ();

    //Send a message on the serial link
    void
    dprintln(String s);

    //die
    void
    die();

    /** ------------------
     *  THREADS
     * --------------------- */

    //Do not call this function before OS is initialized
    //Create a new thread pointing on a function (C style)
    void
    createThread_C (const char * const name, ThreadRunFct runFunction,
		    uint16_t stack, uint16_t priority);

    //Do not call this function before OS is initialized
    //Create a new thread pointing on a class method (C++ style)
    void
    createThread_Cpp (const char * const name, IThread& pClass, uint16_t stack,
		      uint16_t priority);

    //Do not call this function before OS is initialized
    //Create a new thread pointing on a class method (C++ style)
    //The run method will be called periodically
    void
    createPeriodicThread_Cpp (const char * const name, IThread& pClass, uint16_t stack,
		      uint16_t priority, uint16_t periodMs);

    /** ------------------
     *  SIGNAL
     * --------------------- */

    //Create a Signal instance, and update statistics
    Signal
    Signal_create ();

    //Signal to the signal listener that he can awake
    //Don't put a fucking NULL pointer, it's not checked
    //Only available when ArdOs is running (else it assets)
    void
    Signal_set (Signal s);

    //Wait until the signal is set. Usually only one thread
    //should wait at a time. Note that if several wait are called
    //in a raw, the next set will only awake one waiter
    //Don't put a fucking NULL pointer, it's not checked
    //Only available when ArdOs is running (else it assets)
    void
    Signal_wait (Signal s);

    /** ------------------
     *  MUTEX
     * --------------------- */

    //Create a Mutex instance, and update statistics it should be used to
    //protect shared memories. Do not use this for thread
    //synchronization, prefer a Signal for that use.
    Mutex
    Mutex_create ();

    //Take a critical section.
    //Only available when ArdOs is running (else it assets)
    //Don't put a fucking NULL pointer, it's not checked
    void
    Mutex_lock (Mutex m);

    //Release a critical section. It should be used to
    //protect shared memories. Do not use this for thread
    //synchronization, prefer a Signal for that use.
    //Only available when ArdOs is running (else it assets)
    //Don't put a fucking NULL pointer, it's not checked
    void
    Mutex_unlock (Mutex m);

    /** ------------------
     *  SEMAPHORE
     * --------------------- */

    //Create a Semaphore instance, register it for statistics
    //@param maxCount : maximal number of items in the semaphore
    //@param initCount : number of items "gived" to the semaphore at initialization
    Semaphore
    Semaphore_create (const UBaseType_t maxCount, const UBaseType_t initCount);

    //Put a new resource in the semaphore
    //Only available when ArdOs is running (else it assets)
    void
    Semaphore_give (Semaphore s);

    //Get a new resource in the semaphore
    //Only available when ArdOs is running (else it assets)
    void
    Semaphore_take (Semaphore s);

    //Try to get new resource in the semaphore, don't block on failure
    //@return true when taken, false when semaphore is empty
    //Only available when ArdOs is running (else it assets)
    bool
    Semaphore_tryTake (Semaphore s);

  private:

    //singleton instance
    static ArdOs instance;

    //table of all threads handlers
    TaskHandle_t* threads[configMAX_PRIORITIES];

    //statistic counter for signals
    uint16_t signalCount;

    //statistic counter for mutex
    uint16_t mutexCount;

    //table of all params (because they have to be static)
    genericRunParams params[configMAX_PRIORITIES];

    //next rank in the table
    uint8_t nextThreadRank;

    //strictly incrementing counter
    uint32_t heartbeatCounter;

    //last value sent to the heartbeat LED;
    uint8_t heartbeatPinValue;

    eOsState state;

    //Date of boot (ie time at which the scheduler is started)
    TimeMs bootDuration;

    //mutex to protect the debug serial link
    Mutex debugSerialMutex;

    //private constructor as its a singleton class
    ArdOs ();COPY_CONSTRUCTORS (ArdOs)
    ;
  };

}	//end namespace

#endif /* ROBOTS_ARDOS_H_ */
