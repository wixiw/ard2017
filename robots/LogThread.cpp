/*
 * LogThread.cpp
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#include "LogThread.h"
#include "K_thread_config.h"
#include "K_pinout.h"

using namespace ard;

//singleton instanciation
LogThread LogThread::instance = LogThread();

LogThread::LogThread () :
    configSerialLog (true), configSdCardLog (true), semDataPresent (NULL), semFreeSpace (
    NULL), fifoHead (0), fifoTail (0), fifoCount (0), missedLogs (0), sdCardPresent (
	false)
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

  //setup the SDcard driver and open the log file
  initSDcard ();
}

void
LogThread::initSDcard ()
{
  Serial.println ("[LogThread] Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin (CS_SDCARD))
    {
      Serial.println (
	  "[LogThread] SD card init FAILED, or not present, log persistency disabled.");
    }
  else
    {
      sdCardPresent = true;
      Serial.println ("[LogThread] SD card init SUCCESS.");
    }
}

void
LogThread::run ()
{

  while (1)
    {
      //If the logger is disabled, wait until it is enable again
      if (!configSerialLog && !configSerialLog)
	{
	  vTaskDelay (1000);
	}
      //Else take the next item in the fifo (or wait until one is here)
      else
	{
	  unpileFifo ();
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
  ++fifoCount;

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

  if (configSdCardLog && sdCardPresent)
    {
      //If file is not opened yet ... open it !
      if (!logFile)
	{
	  // open the file. note that only one file can be open at a time,
	  // so you have to close this one before opening another
	  // (maybe it's not true anymore in recent lib version...)
	  logFile = SD.open ("log.txt", FILE_WRITE);

	  //if the file is not opened, consider that the sdcard is not present
	  if (!logFile)
	    {
	      Serial.println (
		  "[FATAL] Failed to open log file, SDcard logging feature disabled.");
	      sdCardPresent = false;
	    }
	}

      //Write to the file
      logFile.println (formatLogMsg (fifoArray[fifoTail]));

      //When there is no pending log, close the file to prevent loosing data and corrupting the filesystem.
      //1 because the counter is not decremented yet
      if (fifoCount <= 1)
	{
	  logFile.close ();
	}
    }

  // release record
  --fifoCount;
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
