/*
 * LogThread.cpp
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#include "LogThread.h"
using namespace ard;

//Singleton instanciation
LogThread LogThread::instance=LogThread();

LogThread::LogThread():
	configSerialLog(true),
	configSdCardLog(true)
{
	//TODO
	//create the queue, and associated semaphores/mutex
	//PRIO_LOG_SD
	//PRIO_LOG_SERIAL
}

void LogThread::run()
{
	//TODO unpile the queue

	if( instance.configSerialLog )
	{
		//TODO send to serial
	}

	if( instance.configSdCardLog )
	{
		//TODO send to spi
	}
}

void LogThread::log(eLogLevel logLevel, char const * const log)
{
	//empiler le message dans la queue
}
