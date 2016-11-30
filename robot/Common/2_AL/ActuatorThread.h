/*
 * ActuatorThread.h
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ACTUATORTHREAD_H_
#define ROBOTS_ACTUATORTHREAD_H_

#include "RSP.h"

namespace ard
{
  class Robot2017;

  class ActuatorThread : public IThread
  {
  public:
    ActuatorThread () = default;
    virtual
    ~ActuatorThread () = default;

    //Implements IThreads : create the thread
    void init() override;

    //Implements IThreads : run all mini threads (nav, actuator systems)
    void
    run () override;

    //Add a mini thread to the list, shall be called before calling init()
    void
    addMiniThread(IMiniPeriodicThread* threadlet );

  private:
    static const uint8_t NB_MAX_MINI_THREADS = 1;
    IMiniPeriodicThread* threadlets[NB_MAX_MINI_THREADS];
    uint8_t nextRank;
  };

} /* namespace ard */

#endif /* ROBOTS_ACTUATORTHREAD_H_ */
