/*
 * StrategyThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_LIFECYCLE_H_
#define ROBOTS_LIFECYCLE_H_

#include "RSP.h"

#ifdef BUILD_STRATEGY

#define NB_MAX_STRATEGIES 15

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

	/**
	 * This class manages the robot highest functionnal level. The global state machine named Lifecycle
	 * manages robots "modes". In a mode some actions are allowed and other are not
	 */
  class Lifecycle : public Thread, public EventListener
  {
  public:
    Lifecycle();
    void attachRobot(Robot2017* robot);

    /**
     * -----------------------------
     */

    //Override Thread : create the event
    void init() override;

    //Implements Thread : executes the stategy
    void run() override;
	
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
	uint8_t nbRegisteredStrats;

	Robot2017* robot;
  };

} /* namespace ard */

#endif //build strategy

#endif /* ROBOTS_LIFECYCLE_H_ */
