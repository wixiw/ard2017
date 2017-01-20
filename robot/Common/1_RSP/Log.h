/*
 * Log.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_LOGTHREAD_H_
#define ROBOTS_LOGTHREAD_H_

#include "ArdOs.h"

namespace ard
{
    //fifo item
    struct LogMsg
    {
        TimeMs date;
        eLogLevel level;
        char component[configMAX_TASK_NAME_LEN];
        char text[LOG_MAX_SIZE];
    };

    /**
     * Interface of a log channel ie a media where the log is written
     */
    class ILogChannel
    {
    public:
        virtual ~ILogChannel() = default;

        //push a log to the RAM buffer, the log will only be effective when the LogDispatcher will have read the buffer
        virtual void log(LogMsg const & log) = 0;

        //returns true if the channel is ready to log, false if it can't log.
        //when it's false, any call to log is silently ignored
        virtual bool isReady() const = 0;
    };

    /**
     * Write a log on an SD Card
     * This class is very limited with connection/unconnection. You cannot hotplug SDcard.
     * Likewise, if an error occurs with file opening, this class stops its activity.
     */
    class SdCardLogger: public Thread, public ILogChannel
    {
    public:
        SdCardLogger(uint8_t recvQueueSize);
        virtual ~SdCardLogger() = default;

        //Implements Thread : try to open the SD card and reads the serial inputs
        void run() override;

        //Implements ILogChannel : write the log on the SD card
        virtual void log(LogMsg const & log) override;

        //Implements ILogChannel : returns true if the SCCard is connected and opening succeed in init()
        virtual bool isReady() const override {return sdCardPresent;};

    private:
        //Detection of SDcard presence
        bool sdCardPresent;

        //Log file
        File logFile;

        //use to format a serial log
        String formatLogMsg(LogMsg const& msg);

        //receive queue to decouple sender from SD card blocking calls
        Queue queue;

        //SC card write buffer to unpile queue
        LogMsg writeBuffer;
    };

    //alias to get ArdOs singleton instance
    #define LOG_DEBUG(msg)  LogDispatcher::getInstance().log(eLogLevel_DEBUG,msg)
    #define LOG_INFO(msg)   LogDispatcher::getInstance().log(eLogLevel_INFO,msg)
    #define LOG_ERROR(msg)  LogDispatcher::getInstance().log(eLogLevel_ERROR,msg)
    #define LOG_ASSERT(msg) LogDispatcher::getInstance().log(eLogLevel_ASSERT,msg)

    /**
     * This class is used to decouple log providers
     * from log writters. In general the provider is a
     * real time task, and the writter may have some delay
     * as long as the logs keep the provided order.
     *
     * This class sends logs to the serial line and to an spi sd card.
     * It's highly adviced that the thread has the lowest priority in the system.
     *
     * Logs are flushed in the sdcard each time there is no pending log
     * so that :
     *  - writing performance are good due to the fact data are written in groups
     *  - file is closed regurlarly to prevent filesystem corruption.
     *
     */
    class LogDispatcher: public ILogger
    {
    public:
        virtual ~LogDispatcher() = default;

        //retrieve the singleton instance (you should prefer the use of LOG_xxx macros)
        static LogDispatcher& getInstance()
        {
            return instance;
        };

        //Implements ILogger : publish the log to all existing logChannels
        void log(eLogLevel logLevel, String const& log) override;

        //attach a new com logger
        void addLogger(ILogChannel& channel);

    private:
        //singleton instance
        static LogDispatcher instance;

        //Com logger
        #define MAX_LOGGERS 2
        uint8_t nbLoggers;
        ILogChannel* loggers[MAX_LOGGERS];

        //helper to send a log to all connected channels
        void disptachLogToChannels(LogMsg const& log);

        //private constructor as its a singleton class
        LogDispatcher();COPY_CONSTRUCTORS (LogDispatcher)
    };
}

#endif /* ROBOTS_LOGTHREAD_H_ */
