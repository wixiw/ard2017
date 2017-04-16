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

    /**
     * ----  User API  --------------
     */

    //Inform strategy that a cylinder has been withdrawn from our mono-color dispenser, 0 means all available
    void informWithdraw_MonocolorDispenser(uint8_t nb = 0);

    //Inform strategy that a cylinder has been withdrawn from our bi-color dispenser, 0 means all available
    void informWithdraw_BicolorDispenser(uint8_t nb = 0);

    //Inform strategy that a cylinder has been withdrawn from opponent bi-color dispenser, 0 means all available
    void informWithdraw_OppDispenser(uint8_t nb = 0);

    //Inform strategy that a cylinder has been pooed in the middle center container, and decrease stock count, 0 means all available
    void informPooed_MiddleCenter(uint8_t nb = 0);

    //Inform strategy that a cylinder has been pooed in the middle container on our side, and decrease stock count, 0 means all available
    void informPooed_MiddleOwn(uint8_t nb = 0);

    //Inform strategy that a cylinder has been pooed in the middle container on opponent side, and decrease stock count, 0 means all available
    void informPooed_MiddleOpp(uint8_t nb = 0);

    //Inform strategy that a cylinder has been pooed in the border container on our side, and decrease stock count, 0 means all available
    void informPooed_Border(uint8_t nb = 0);

    //Inform strategy that a cylinder has been pooed in the border container on opponent side, and decrease stock count, 0 means all available
    void informPooed_BorderOpp(uint8_t nb = 0);

    //Inform strategy that a cylinder has been pooed in our start area, and decrease stock count, 0 means all available
    void informPooed_Start(uint8_t nb = 0);

    //Inform strategy that a cylinder has been pooed on table, 0 means all available
    void informPooed_OnTable(uint8_t nb = 0);

    //Inform stategy that a cylinder has been taken on table (close to our start position)
    void informTaken_Start();

    //Inform stategy that a cylinder has been taken on table (close to our midle center container)
    void informTaken_Container();

    //Inform stategy that a cylinder has been taken on table (in the middle of our table side)
    void informTaken_Center();

    //Inform stategy that a cylinder has been taken on table (close to our start position table corner)
    void informTaken_Corner();

    //Inform stategy that a cylinder has been taken on table (close to our bottom craters)
    void informTaken_Crater();

    //Inform stategy that a cylinder has been taken on table (close to opponent start position)
    void informTaken_OppStart();

    //Inform stategy that a cylinder has been taken on table (close to opponent midle center container)
    void informTaken_OppContainer();

    //Inform stategy that a cylinder has been taken on table (in the middle of opponent table side)
    void informTaken_OppCenter();

    //Inform stategy that a cylinder has been push away from its default position (close to our start position)
    void informPushedAway_Start();

    //Inform stategy that a cylinder has been push away from its default position (close to our midle center container)
    void informPushedAway_Container();

    //Inform stategy that a cylinder has been push away from its default position (in the middle of our table side)
    void informPushedAway_Center();

    //Inform stategy that a cylinder has been push away from its default position (close to our start position table corner)
    void informPushedAway_Corner();

    //Inform stategy that a cylinder has been push away from its default position (close to our bottom craters)
    void informPushedAway_Crater();

    //Inform stategy that a cylinder has been push away from its default position (close to opponent start position)
    void informPushedAway_OppStart();

    //Inform stategy that a cylinder has been push away from its default position (close to opponent midle center container)
    void informPushedAway_OppContainer();

    //Inform stategy that a cylinder has been push away from its default position (in the middle of opponent table side)
    void informPushedAway_OppCenter();

    /**
     * -----------------------------
     */

    //Override Thread : create the event
    void init() override;

    //Implements Thread : executes the stategy
    void run () override;
	
	//register new strategy in the list
	void 
	registerStrategy(String name, StrategyFunctor functor);

	//force the match configuration without using the HMI
	void configureMatch(uint8_t strategyId, eColor matchColor);

	//Use by telemetry to get fresh data
	apb_StratInfo2017 const& getStratInfo();

  private:
    //read user config (color and strat selection)
    void readUserInputs();

    //display preamble information to the user
    //it's just a mean to reduce the volume of code in the run function
    void displayIntroduction();

    //helper function for updating stratInfo
    void withdraw(uint8_t nb, uint32_t& dispenserCount, String const& caller);
    void poo(uint8_t nb, uint8_t max, uint32_t& containerCount, String const& caller);
    void take(bool& objectPresent, String const& caller);
    void push(bool& objectPresent, const String& caller);

    //a value identifying the strategy choosed by the user
    uint8_t strategyId;
	
	//list of strategies
	StrategyDescriptor strategies[NB_MAX_STRATEGIES];

	//cache to hold telemetry data
	apb_StratInfo2017 stratInfo;

	Robot2017* robot;
  };

} /* namespace ard */

#endif //build strategy

#endif /* ROBOTS_TELEOPTHREAD_H_ */
