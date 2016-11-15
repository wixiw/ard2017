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
    void init() override;

    //Implements IThreads : executes the stategy
    void
    run () override;

  private:
    //for debug only : light a led for each activated user switch
    void Strategy_ButtonTest();
    //for debug only : light a led for each activated omron
    void Strategy_OmronTest();
    //for debug only : make all leds blinking to check they are working
    void Strategy_LedTest();

    Robot2017& r;
  };

} /* namespace ard */

#endif /* ROBOTS_TELEOPTHREAD_H_ */
