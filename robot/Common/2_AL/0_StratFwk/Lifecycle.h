/*
 * StrategyThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_LIFECYCLE_H_
#define ROBOTS_LIFECYCLE_H_

#include "RSP.h"
#include "StratInterfaces.h"

#define private protected //workaround to gain instropection
#include "generated/FSM_Lifecycle.h"
#undef private

#ifdef BUILD_STRATEGY

#define NB_MAX_STRATEGIES 15

namespace ard
{
    class Robot2017;

    //Every strategy function shall have this signature :
    typedef void (*StrategyFunctor)(Robot2017& robot);

	//Used to display the list of existing strategies
	struct MatchDescriptor{
		String name;
		IStrategy* install;
		IStrategy* match;
		IStrategy* funny;
		StrategyFunctor linear;
	};

	/**
	 * This class is used to create the instrospection that Yakindu refuses to do
	 */
	class FSM_Lifecycle_Better: public FSM_Lifecycle
	{
	public:
	    FSM_Lifecycle_Better();
	    void run();
	    FSM_LifecycleStates getState() const {return stateConfVector[0];};
	    String state2Str(FSM_LifecycleStates state) const;
	protected:
	    FSM_LifecycleStates lastState;
	};

	/**
	 * This class manages the robot highest functionnal level. The global state machine named Lifecycle
	 * manages robots "modes". In a mode some actions are allowed and other are not
	 */
  class Lifecycle : public Thread, public FSM_Lifecycle::DefaultSCI_OCB
  {
  public:
    Lifecycle(Robot2017* robot);

    /**
     * -----------------------------
     */

    //Override Thread : create the event
    void init() override;

    //Implements Thread : executes the stategy
    void run() override;
	
	//register new match in the list
    //@param : the name of the strategy, shall not be empty
    //@param install : the strategy to run before the match to prepare the robot, or NULL to skip
    //@param match : the strategy to run during the match, or NULL to skip
    //@param funny : the funny action to run at the match end, or NULL to skip
	void registerMatchType(String const& name, IStrategy* install, IStrategy* match, IStrategy* funny);

	//register a new simple linear (not periodic) strategy.
	//For backward compatibility with previous design
	void registerLinearStrat(String const& name, StrategyFunctor functor);

	//automatic robot configuration (network command from vizu)
	void networkConfigRequest(uint8_t strategyId_, eColor matchColor);

	//automatic match start (network command from vizu)
	void startMatch();

	//robot strategy may request to end the match sooner than expected
	void endMatch();

	//robot strategy may request to end the funny action sooner than expected
	void endFunnyAction();

  protected:
    //Updates FSM inputs
    void readInputs();

    //Take into account FSM outputs
    void publishOutputs();

    //display preamble information to the user
    //it's just a mean to reduce the volume of code in the run function
    //FSM callback
    void displayIntroduction();

    //FSM callback
    void beep(sc_integer nb);

    //force the match configuration without using the HMI
    void configureMatch(uint8_t strategyId, eColor matchColor);

    //a value identifying the strategy choosed by the user
    uint8_t strategyId;
	
	//list of strategies
	MatchDescriptor matchs[NB_MAX_STRATEGIES];
	uint8_t nbRegisteredStrats;

	//State machine behavior
	FSM_Lifecycle_Better fsm;

	Robot2017* robot;
  };

} /* namespace ard */

#endif //build strategy

#endif /* ROBOTS_LIFECYCLE_H_ */
