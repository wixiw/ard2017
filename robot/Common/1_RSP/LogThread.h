/*
 * LogThread.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_LOGTHREAD_H_
#define ROBOTS_LOGTHREAD_H_

#include "ArdUtils.h"
#include "ArdOs.h"
#include "BSP.h"
#include "Types.pb.h"

namespace ard
{
    //fifo item
    struct LogMsg
    {
        TimeMs date;
        eLogLevel level;
        String text;
    };

    /**
     * Interface of a log channel ie a media where the log is written
     */
    class ILogChannel
    {
    public:
        virtual ~ILogChannel() = default;

        //push a log to the RAM buffer, the log will only be effective when the LogThread will have read the buffer
        virtual void log(LogMsg const & log) = 0;

        //returns true if the channel is ready to log, false if it can't log.
        //when it's false, any call to log is silently ignored
        virtual bool isReady() const = 0;

    };

    //Write a log on an SD Card
    class SdCardLogger: public ILogChannel
    {
    public:
        virtual ~SdCardLogger() = default;

        //Implements ILogChannel : open the SD card file
        //setup the SDcard driver and open the log file
        //shall not be done in the init section (...) because it relies on millis() calls to check timeout which only works when the OS is started.
        virtual void init();

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
    };

//alias to get ArdOs singleton instance
#define LOG_DEBUG(msg) LogThread::getInstance().log(eLogLevel_DEBUG,msg)
#define LOG_INFO(msg) LogThread::getInstance().log(eLogLevel_INFO,msg)
#define LOG_ERROR(msg) LogThread::getInstance().log(eLogLevel_ERROR,msg)


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
    class LogThread: public IThread
    {
    public:
        virtual ~LogThread() = default;

        //retrieve the singleton instance (you should prefer the use of the g_ArdOs maccro)
        static LogThread&
        getInstance()
        {
            return instance;
        }
        ;

        //Implements IThreads : Create threads, configures the serial line and open the file
        void
        init() override;

        //Implements IThreads : unpile the fifo
        void
        run() override;

        //push a log to the RAM buffer, the log will only be effective when the LogThread will have read the buffer
        void
        log(eLogLevel logLevel, String const& log);

        //attach a new com logger
        void setComLogger(ILogChannel* channel)
        {
            comLogger = channel;
        }

        //configure if logs are sent to the serial bus (activated by default)
        void activateSDLogs(bool activate)
        {
            configSdCardLog = activate;
        }

    private:
        bool configSdCardLog;

        //fifo index
        typedef uint16_t FifoIndex;

        //singleton instance
        static LogThread instance;

        // count of data records in fifo
        Semaphore semDataPresent;

        // ensure only one log is pile at a time
        Mutex mutex;

        // size of fifo in records
        static const FifoIndex FIFO_SIZE = 20;

        // array of data items
        LogMsg fifoArray[FIFO_SIZE];

        //index of the first element
        FifoIndex fifoHead;

        //index of the first element
        FifoIndex fifoTail;

        //Nb pending logs
        FifoIndex fifoCount;

        //Counter of lost logs due to the fact that the log thread is too slow
        uint8_t missedLogs;

        //File logger
        SdCardLogger fileLogger;

        //Com logger
        ILogChannel* comLogger;

        //to be called periodically in the log thread
        void
        unpileFifo();

        //helper to send a log to all connected channels
        void disptachLogToChannels(LogMsg const& log);

        //private constructor as its a singleton class
        LogThread();COPY_CONSTRUCTORS (LogThread)
    };
}

#endif /* ROBOTS_LOGTHREAD_H_ */
