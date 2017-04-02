/*
 * StrategyThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_STRATEGYTHREAD_H_
#define ROBOTS_STRATEGYTHREAD_H_

#include "RSP.h"

#ifdef BUILD_STRATEGY

#define NB_MAX_STRATEGIES 4

namespace ard
{
    class Robot2017;

    //Every strategy function shall have this signature :
	typedef void (*StrategyFunctor)(Robot2017& robot);

	//Used to display the list of existing strategies
	struct StrategyDescriptor{
		String name;
		StrategyFunctor functor;
	};

  class StrategyThread : public Thread, public EventListener
  {
  public:
    StrategyThread();
    void attachRobot(Robot2017* robot);

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

	Robot2017* robot;
  };

} /* namespace ard */

#endif //build strategy

#endif /* ROBOTS_TELEOPTHREAD_H_ */
