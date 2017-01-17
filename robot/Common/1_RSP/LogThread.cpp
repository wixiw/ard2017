/*
 * LogThread.cpp
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#include "LogThread.h"
#include "K_thread_config.h"
#include "BSP.h"

using namespace ard;

//singleton instanciation
LogThread LogThread::instance = LogThread();

void SdCardLogger::connect()
{
    // see if the card is present and can be initialized:
    if (SD.begin (CS_SDCARD))
    {
        sdCardPresent = true;
    }
}

void SdCardLogger::log(LogMsg const & log)
{
    if (sdCardPresent)
    {
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another
        // (maybe it's not true anymore in recent lib version...)
        logFile = SD.open("log.txt", FILE_WRITE);

        //if the file is not opened, consider that the sdcard is not present
        if (!logFile)
        {
            sdCardPresent = false;
        }

        //Write to the file
        logFile.println(formatLogMsg(log));

        //close the file to prevent loosing data and corrupting the filesystem on HW disconnection.
        logFile.close();
    }
}

String SdCardLogger::formatLogMsg(LogMsg const& msg)
{
    String level;
    switch (msg.level)
    {
    case eLogLevel_DEBUG:
        level = "DEBUG";
        break;
    case eLogLevel_INFO:
        level = "INFO";
        break;
    default:
    case eLogLevel_ERROR:
        level = "ERROR";
        break;
    }

    return String(msg.date) + " [" + level + "] [" + msg.component + "] " + msg.text;
}

LogThread::LogThread():
        Thread("Log", PRIO_LOG, STACK_LOG),
        configSdCardLog(true),
        semDataPresent(NULL),
        mutex(),
        fifoHead(0),
        fifoTail(0),
        fifoCount(0),
        missedLogs(0),
        comLogger(NULL)
{
    INIT_TABLE_TO_ZERO(fifoArray);
}

void LogThread::init()
{
    Thread::init();

    // initialize fifoData semaphore to no data available
    semDataPresent = xSemaphoreCreateCounting(FIFO_SIZE, 0);
}

void LogThread::run()
{
    fileLogger.connect();

    if( fileLogger.isReady() )
    {
        LogMsg msg;
        msg.date = 0;
        msg.level = eLogLevel_INFO;
        msg.component = getName();
        msg.text = "SDCard log enabled.";
        disptachLogToChannels(msg);
    }

    while (1)
    {
        //If all log channel are disabled, wait until one is enabled again
        if (comLogger==NULL && !configSdCardLog)
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

void LogThread::log(eLogLevel logLevel, String const& log)
{
    //debug logs are not even piled if DEBUG is not active.
#ifndef ARD_DEBUG
    if (logLevel == eLogLevel_DEBUG)
        return;
#endif

    //take the time of log before any action
    auto now = millis();

    mutex.lock();

    // get a buffer in the fifo to fill the message
    if (fifoCount == FIFO_SIZE)
    {
        // fifo full - indicate missed point (unless there already many missed logs)
        if (missedLogs < 255)
            missedLogs++;
        return;
    }

    // fill the buffer
    fifoArray[fifoHead].date = now;
    fifoArray[fifoHead].level = logLevel;
    fifoArray[fifoHead].component =  pcTaskGetTaskName(xTaskGetCurrentTaskHandle());
    fifoArray[fifoHead].text = log;
    ++fifoCount;

    // advance FIFO index
    fifoHead = fifoHead < (FIFO_SIZE - 1) ? fifoHead + 1 : 0;

    // signal a new data is in the fifo
    xSemaphoreGive(semDataPresent);

    mutex.unlock();
}

void LogThread::unpileFifo()
{
    if (missedLogs)
    {
        LogMsg msg;
        msg.date = millis();
        msg.level = eLogLevel_ERROR;

        if (missedLogs == 255)
            msg.text = "[FATAL] More than 255 logs have been lost.";
        else
            msg.text = String ("[FATAL] ") + missedLogs + " logs have been lost.";
        disptachLogToChannels(msg);
        missedLogs = 0;
    }

    // wait for next data record
    xSemaphoreTake(semDataPresent, portMAX_DELAY);

    //dispatch log
    disptachLogToChannels(fifoArray[fifoTail]);

    mutex.lock();
    // release record
    --fifoCount;
    // advance FIFO index
    fifoTail = fifoTail < (FIFO_SIZE - 1) ? fifoTail + 1 : 0;
    mutex.unlock();
}

void LogThread::disptachLogToChannels(LogMsg const& log)
{
    if(comLogger != NULL)
        comLogger->log(log);

    if (configSdCardLog)
        fileLogger.log(log);
}


