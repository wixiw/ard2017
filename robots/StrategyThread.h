/*
 * StrategyThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_STRATEGYTHREAD_H_
#define ROBOTS_STRATEGYTHREAD_H_

#include "ArdOs.h"

namespace ard
{
  class Robot2017;

  class StrategyThread : public IThread
  {
  public:
    StrategyThread (Robot2017& robot);
    virtual
    ~StrategyThread ()
    {
    }
    ;

    //Implements IThreads : create the thread
    void init();

    //Implements IThreads : ...
    void
    run ();

  private:
    Robot2017& r;
  };

} /* namespace ard */

#endif /* ROBOTS_TELEOPTHREAD_H_ */
