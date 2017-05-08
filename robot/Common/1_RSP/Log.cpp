/*
 * Log.cpp
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#include "K_thread_config.h"
#include "Log.h"

using namespace ard;

//singleton instanciation
LogDispatcher LogDispatcher::instance = LogDispatcher();

SdCardLogger::SdCardLogger(uint8_t recvQueueSize):
        Thread("SDCard", PRIO_SDCARD_WRITER, STACK_SDCARD_WRITER),
        sdCardPresent(false),
        logFile(),
        queue(recvQueueSize, sizeof(LogMsg)),
        writeBuffer()
{}

void SdCardLogger::run()
{
    // see if the card is present and can be initialized:
    if(SD.begin (CS_SDCARD))
    {
        LOG_INFO("SDCard opened.");
        sdCardPresent = true;

        while(666)
        {
            queue.pop(&writeBuffer);

            // open the file. note that only one file can be open at a time,
            // so you have to close this one before opening another
            // (maybe it's not true anymore in recent lib version...)
            logFile = SD.open("log.txt", FILE_WRITE);

            //if the file is not opened, consider that the sdcard is not present
            if(!logFile)
            {
                sdCardPresent = false;
                break;
            }

            logFile.println(formatLogMsg(writeBuffer));

            //close the file to prevent loosing data and corrupting the filesystem on HW disconnection.
            logFile.close();
        }
    }

    LOG_INFO("SDCard absent.");
}


void SdCardLogger::log(LogMsg const & log)
{
    if(sdCardPresent)
    {
        queue.push((void*)&log);
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

LogDispatcher::LogDispatcher():
        nbLoggers(0),
        debugActive(false)
{
    INIT_TABLE_TO_ZERO(loggers);
}

void LogDispatcher::log(eLogLevel logLevel, String const& log)
{
    //debug logs are not even piled if DEBUG is not active.
    if (!debugActive && logLevel == eLogLevel_DEBUG)
        return;

    //take the date of the log before any action
    auto now = millis();
    LogMsg msg;

    // fill the buffer
    msg.date = now;
    msg.level = logLevel;
    TaskHandle_t currentTask = xTaskGetCurrentTaskHandle();
    if(currentTask)
        strcpy(msg.component, pcTaskGetTaskName(currentTask));
    else
        strcpy(msg.component, "Boot");
        
    strcpy(msg.text, log.c_str());

    disptachLogToChannels(msg);
}

void LogDispatcher::setDebugLevel(bool on)
{
    if(on != debugActive)
    {
        debugActive = on;
        
        if(on)
            LOG_DEBUG("Debug logs activated");
        else
            LOG_INFO("Debug logs deactivated");  
    }
    
}

void LogDispatcher::addLogger(ILogChannel& channel)
{
    ASSERT(nbLoggers < MAX_LOGGERS);
    //append the channel to the list and increment logger count
    loggers[nbLoggers++] = &channel;
}

void LogDispatcher::disptachLogToChannels(LogMsg const& log)
{
    for( int i = 0 ; i < nbLoggers ; i++ )
    {
        loggers[i]->log(log);
    }
}


