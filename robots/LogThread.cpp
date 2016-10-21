/*
 * LogThread.cpp
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#include "LogThread.h"
#include "K_thread_config.h"

using namespace ard;

LogThread::LogThread () :
    configSerialLog (true), configSdCardLog (true), semDataPresent (NULL), semFreeSpace (
    NULL), fifoHead (0), fifoTail (0), missedLogs (0)
{
  INIT_TABLE_TO_ZERO(fifoArray);
}

void
LogThread::init ()
{
  //create the thread
  g_ArdOs.createThread_Cpp("Log", *this, STACK_LOG, PRIO_LOG);

  // initialize fifoData semaphore to no data available
  semDataPresent = g_ArdOs.Semaphore_create(FIFO_SIZE, 0);

  // initialize fifoSpace semaphore to FIFO_SIZE free records
  semFreeSpace = g_ArdOs.Semaphore_create(FIFO_SIZE, FIFO_SIZE);
}

void
LogThread::run ()
{
  while (1)
    {
      //If the logger is disabled, wait until it is enable again
      if (!configSerialLog && !configSerialLog )
	{
	  vTaskDelay(1000);
	}
      //Else take the next item in the fifo (or wait until one is here)
      else
	{
	  unpileFifo();
	}
    }
}

void
LogThread::log (eLogLevel logLevel, String const& log)
{
  //take the time of log before any action
  TimeMs now = millis ();

  // get a buffer in the fifo to fill the message
  if (!g_ArdOs.Semaphore_tryTake(semFreeSpace))
    {
      // fifo full - indicate missed point (unless there already many missed logs)
      if (missedLogs < 255)
	missedLogs++;

      return;
    }

  // fill the buffer
  fifoArray[fifoHead].date = now;
  fifoArray[fifoHead].level = logLevel;
  fifoArray[fifoHead].text = log;

  // signal a new data is in the fifo
  g_ArdOs.Semaphore_give(semDataPresent);

  // advance FIFO index
  fifoHead = fifoHead < (FIFO_SIZE - 1) ? fifoHead + 1 : 0;
}

void
LogThread::unpileFifo ()
{
  if (missedLogs)
    {
      if (missedLogs == 255)
	Serial.println ("[FATAL] More than 255 logs have been lost.");
      else
	Serial.println (
	    String ("[FATAL] ") + missedLogs + " logs have been lost.");
      missedLogs = 0;
    }

  // wait for next data record
  g_ArdOs.Semaphore_take(semDataPresent);

  if (configSerialLog)
    {
      Serial.println (formatLogMsg (fifoArray[fifoTail]));
    }

  if (configSdCardLog)
    {
      //TODO send to spi
    }

  // release record
  g_ArdOs.Semaphore_give(semFreeSpace);

  // advance FIFO index
  fifoTail = fifoTail < (FIFO_SIZE - 1) ? fifoTail + 1 : 0;
}

String
LogThread::formatLogMsg (LogMsg const& msg)
{
  String level;
  switch (msg.level)
    {
    case DEBUG:
      level = "DEBUG";
      break;
    case INFO:
      level = "INFO";
      break;
    default:
    case ERROR:
      level = "ERROR";
      break;
    }

  return String (msg.date) + " [" + level + "] " + msg.text;
}
