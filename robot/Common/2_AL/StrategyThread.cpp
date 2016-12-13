/*
 * StrategyThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "StrategyThread.h"
#include "LogThread.h"
#include "Robot2017.h"

using namespace ard;

StrategyThread::StrategyThread()
{
	INIT_TABLE_TO_ZERO(strategies);
}

void
StrategyThread::init ()
{
  //create the thread
  g_ArdOs.createThread_Cpp("Strategy", *this, STACK_STRATEGY, PRIO_STRATEGY);
}

void
StrategyThread::run ()
{
  for(int i = 0; i < NB_MAX_STRATEGIES ; ++i)
  {
	LOG(INFO, "Strategy[" + String(i) + "]: " + strategies[i].name);  
  }
	
  LOG(INFO, "STRAT : Waiting strategy/color configuration and start insertion.");
  ROBOT.waitStartPlugged();
  
  readUserInputs();
  
  LOG(INFO, "STRAT : Waiting start withdraw to begin strategy.");
  ROBOT.waitStartWithdraw();
  
  //Execute selected strategy
  strategies[strategyId].functor();
}

void
StrategyThread::registerStrategy(String name, StrategyFunctor functor)
{
	static uint8_t nbRegisteredStrats = 0;
	ardAssert(nbRegisteredStrats < NB_MAX_STRATEGIES, "Too many strategies registered.");
	strategies[nbRegisteredStrats].name = name;
	strategies[nbRegisteredStrats].functor = functor;
	nbRegisteredStrats++;
}

void StrategyThread::readUserInputs()
{
  //Read color input
  if( ROBOT.isPreferedColor() )
    {
    ROBOT.nav.setColor (eColor::PREF);
    ROBOT.setRGBled(YELLOW, ON);
    LOG(INFO, "User has selected PREF (Yellow) color");
    }
  else
    {
    ROBOT.nav.setColor (eColor::SYM);
    ROBOT.setRGBled(BLUE, ON);
    LOG(INFO, "User has selected SYM (Blue) color");
    }

  //Read strat config
  strategyId = ROBOT.getStrategyId();
  ardAssert(strategies[strategyId].functor != 0, "Selected strategy functor is null.");
  LOG(INFO, "User has selected strategy " + strategies[strategyId].name);

}

