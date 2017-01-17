/*
 * StrategyThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_STRATEGYTHREAD_H_
#define ROBOTS_STRATEGYTHREAD_H_

#include "RSP.h"

#define NB_MAX_STRATEGIES 4

namespace ard
{
    //Every strategy function shall have this signature :
	typedef void (*StrategyFunctor)(void);

	//Used to display the list of existing strategies
	struct StrategyDescriptor{
		String name;
		StrategyFunctor functor;
	};

  class StrategyThread : public Thread, public EventListener
  {
  public:
    StrategyThread();

    //Override Thread : create the event
    void init() override;

    //Implements Thread : executes the stategy
    void run () override;
	
	//register new strategy in the list
	void 
	registerStrategy(String name, StrategyFunctor functor);

	//force the match configuration without using the HMI
	void configureMatch(uint8_t strategyId, eColor matchColor);

  private:
    //read user config (color and strat selection)
    void readUserInputs();

    //display preamble information to the user
    //it's just a mean to reduce the volume of code in the run function
    void displayIntroduction();

    //a value identifying the strategy choosed by the user
    uint8_t strategyId;
	
	//list of strategies
	StrategyDescriptor strategies[NB_MAX_STRATEGIES];
  };

} /* namespace ard */

#endif /* ROBOTS_TELEOPTHREAD_H_ */
