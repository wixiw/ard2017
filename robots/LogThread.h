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
#include <SPI.h>
#include <SD.h>

namespace ard
{

  typedef enum
  {
    DEBUG = 0, INFO = 1, ERROR = 2,
  } eLogLevel;

  /**
   * Functionnal interface to ease unit tests
   */
  class ILogger
  {
  public:
    virtual ~ILogger(){};

    //push a log to the RAM buffer, the log will only be effective when the LogThread will have read the buffer
    virtual void
    log (eLogLevel logLevel, String const& log) = 0;
  };

//alias to get ArdOs singleton instance
#define LOG(lvl, msg) LogThread::getInstance().log(lvl,msg)
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
  class LogThread : public IThread, public ILogger
  {
  public:
    virtual
    ~LogThread ()
    {
    }
    ;

    //retrieve the singleton instance (you should prefer the use of the g_ArdOs maccro)
    static LogThread&
    getInstance ()
    {
      return instance;
    }
    ;

    //Create threads, configures the serial line and open the file
    void
    init ();

    //Implements IThreads : unpile the fifo
    void
    run ();

    //Implements ILogger
    void
    log (eLogLevel logLevel, String const& log);

    //configure if logs are sent to the serial bus (activated by default)
    bool configSerialLog;

    //configure if logs are sent to the serial bus (activated by default)
    bool configSdCardLog;

  private:
    //fifo item
    struct LogMsg
    {
      TimeMs date;
      eLogLevel level;
      String text;
    };

    //fifo index
    typedef uint16_t FifoIndex;

    //singleton instance
    static LogThread instance;

    // count of data records in fifo
    Semaphore semDataPresent;

    // count of free buffers in fifo
    Semaphore semFreeSpace;

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

    //Detection of SDcard presence
    bool sdCardPresent;

    //Log file
    File logFile;

    //to be called periodically in the log thread
    void
    unpileFifo ();

    //use to format a serial log
    String
    formatLogMsg (LogMsg const& msg);
    void
    initSDcard ();

    //private constructor as its a singleton class
    LogThread ();COPY_CONSTRUCTORS (LogThread)
  };

//TODO faire une fonction a parametres variables pour mettre des variables
}

#endif /* ROBOTS_LOGTHREAD_H_ */
