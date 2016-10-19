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
#include "K_thread_config.h"
#include "ardUtils.h"

namespace ard {

typedef uint32_t TimeMs;
typedef uint32_t DelayMs;

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
class ArdOs {
public:
	//
	static ArdOs& getInstance(){return instance;};

	//to be called before any action on this class
	void init();

	//start the scheduler, call this after having build your application object instances
	void start();

	//signal the SW is alive. Call this in the lowest priority task (loop())
	void kickHeartbeat();

	//Do not call this function before OS is initialized
	//Create a new thread
	void createThread(const char * const name,
			TaskFunction_t runFunction, uint16_t stack, uint16_t priority);

	//TODO static reportStackSizes
	//TODO static reportCpuConsumption

private:
	//singleton instance
	static ArdOs instance;

	//table of all threads handlers
	TaskHandle_t* threads[configMAX_PRIORITIES];

	//next rank in the table
	uint8_t nextThreadRank;

	//strictly incrementing counter
	uint32_t heartbeatCounter;

	//last value sent to the heartbeat LED;
	uint8_t heartbeatPinValue;

	//private constructor as its a singleton class
	ArdOs();
	COPY_CONSTRUCTORS (ArdOs);
};

/**
 * Use this class to create timeout or fire an action after a delay
 * It's a passive class, based on system clock memorisation
 * The comparaison is done when isFired() is called
 */
class SwTimer {
public:
	SwTimer();

	//Start the timer and load a delay in milliseconds
	void arm(DelayMs delayInMs);

	//Cancel the timer execution
	void cancel();

	//Test is the timer is fired
	bool isFired() const;

private:
	TimeMs m_entryDate;
	DelayMs m_delay;
	bool m_started;
};

/**
 * Use this class to lock a critical memory
 * so that several thread can access it without
 * corrupting the data.
 */
class Mutex {
public:
	Mutex();

	//as constructors may be called outside of the runtime (before main), init() is used for runtime init
	//they could be called before the OS is initialize as
	//it's forbidden to call any OS API before init, this function is used to
	//create OS objects
	bool init();

	//Lock the mutex, so that other threads are blocked if they acquire() after
	void acquire();

	//Release the mutex lock so that blocked thread on acquire() can have their turns
	void release();

private:
	SemaphoreHandle_t m_lock;
	COPY_CONSTRUCTORS (Mutex);
};

/**
 * Use this class to synchronize 2 threads
 * One is waiting the other, the other gives the go
 * Typically used to signal an event (ex : new data in a queue).
 */
class Signal {
public:
	Signal();

	//Call this after the OS is initialized but before using the semaphore
	bool init();
	void wait();

	//Don't call this from interrupt
	void set();

	//Call this from interrupt
	void setFromIsr();

private:
	SemaphoreHandle_t sem;
	COPY_CONSTRUCTORS (Signal);
};

}	//end namespace

#endif /* ROBOTS_ARDOS_H_ */
