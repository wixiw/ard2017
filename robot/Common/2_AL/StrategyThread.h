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
    StrategyThread () = default;
    virtual
    ~StrategyThread () = default;

    //Implements IThreads : create the thread
    void init() override;

    //Implements IThreads : executes the stategy
    void
    run () override;

  private:
    //awaiting a real strategy ...
    void Strategy_Alpha();
    //for debug only : light a led for each activated user switch
    void Strategy_ButtonTest();
    //for debug only : light a led for each activated omron
    void Strategy_OmronTest();
    //for debug only : make all leds blinking to check they are working
    void Strategy_LedTest();

    //read user config (color and strat selection)
    void readUserInputs();

    //a value identifying the strategy choosed by the user
    uint8_t strategyId;
  };

} /* namespace ard */

#endif /* ROBOTS_TELEOPTHREAD_H_ */
