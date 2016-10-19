/*
 * LogThread.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_LOGTHREAD_H_
#define ROBOTS_LOGTHREAD_H_

#include "K_thread_config.h"
#include "ardUtils.h"

namespace ard {

typedef enum {
	DEBUG = 0, INFO = 1, ERROR = 2,
} eLogLevel;

/**
 * This class is used to decouple log providers
 * from log writters. In general the provider is a
 * real time task, and the writter may have some delay
 * as long as the logs keep the provided order.
 *
 * This class sends logs to the serial line and to an spi sd card.
 * It's highly adviced that the thread has the lowest priority in the system.
 *
 * Its a static singleton
 */
class LogThread {
public:
	virtual ~LogThread(){};

	//Create threads, configures the serial line and open the file
	static void init();

	//Thread main function
	static void run();

	//push a log to the RAM buffer, the log will only be effective when the LogThread will have read the buffer
	static void log(eLogLevel logLevel, char const * const log);

	//configure if logs are sent to the serial bus (activated by default)
	bool configSerialLog;

	//configure if logs are sent to the serial bus (activated by default)
	bool configSdCardLog;

private:
	//TODO semaphore, mutex et queue
	//singleton instance
	static LogThread instance;

	//private as it's a singleton
	LogThread();
	COPY_CONSTRUCTORS (LogThread);
};

//TODO en faire un thread
//TODO faire une fonction a parametres variables pour mettre des variables
}

#endif /* ROBOTS_LOGTHREAD_H_ */
