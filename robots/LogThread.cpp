/*
 * LogThread.cpp
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#include "LogThread.h"
#include "K_thread_config.h"

using namespace ard;

//Singleton instanciation
LogThread LogThread::instance = LogThread ();

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
  semDataPresent = xSemaphoreCreateCounting(FIFO_SIZE, 0);
  ardAssert(semDataPresent!=NULL, "Log fifo sem creation problem.");

  // initialize fifoSpace semaphore to FIFO_SIZE free records
  semFreeSpace = xSemaphoreCreateCounting(FIFO_SIZE, FIFO_SIZE);
  ardAssert(semFreeSpace!=NULL, "Log fifo sem creation problem.");
}

void
LogThread::run ()
{
  while (1)
    {
      //If the logger is disabled, wait until it is enable again
      if (!g_Log.configSerialLog && !g_Log.configSerialLog )
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
  if (xSemaphoreTake(semFreeSpace, 0) != pdTRUE)
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
  xSemaphoreGive(semDataPresent);

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
  xSemaphoreTake(semDataPresent, portMAX_DELAY);

  if (configSerialLog)
    {
      Serial.println (formatLogMsg (fifoArray[fifoTail]));
    }

  if (configSdCardLog)
    {
      //TODO send to spi
    }

  // release record
  xSemaphoreGive(semFreeSpace);

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
