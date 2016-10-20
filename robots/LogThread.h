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

namespace ard
{

  typedef enum
  {
    DEBUG = 0, INFO = 1, ERROR = 2,
  } eLogLevel;

//alias to get ArdOs singleton instance
#define g_Log LogThread::getInstance()
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
  class LogThread : public IThread
  {
    friend void
    LogThread_run (void);

  public:
    virtual ~LogThread()
      {};

    //retrieve the singleton instance (you should prefer the use of the g_Log maccro)
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

    //push a log to the RAM buffer, the log will only be effective when the LogThread will have read the buffer
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
    SemaphoreHandle_t semDataPresent;

    // count of free buffers in fifo
    SemaphoreHandle_t semFreeSpace;

    // size of fifo in records
    static const FifoIndex FIFO_SIZE = 20;

    // array of data items
    LogMsg fifoArray[FIFO_SIZE];

    //index of the first element
    FifoIndex fifoHead;

    //index of the first element
    FifoIndex fifoTail;

    //Counter of lost logs due to the fact that the log thread is too slow
    uint8_t missedLogs;

    //to be called periodically in the log thread
    void
    unpileFifo ();

    //use to format a serial log
    String
    formatLogMsg (LogMsg const& msg);

    //private as it's a singleton
    LogThread ();COPY_CONSTRUCTORS (LogThread)
    ;
  };

//TODO faire une fonction a parametres variables pour mettre des variables
}

#endif /* ROBOTS_LOGTHREAD_H_ */
