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

  class StrategyThread : public IThread
  {
  public:
    StrategyThread ();
    virtual
    ~StrategyThread ()
    {
    }
    ;

    //create the thread
    void init();

    //Implements IThreads : ...
    void
    run ();
  };

} /* namespace ard */

#endif /* ROBOTS_TELEOPTHREAD_H_ */
