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
  ROBOT.hmi.start.wait(RISING_EDGE);
  
  readUserInputs();
  
  LOG(INFO, "STRAT : Waiting start withdraw to begin strategy.");
  ROBOT.hmi.start.wait(FALLING_EDGE);
  
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
  if( ROBOT.hmi.matchColor.read() == GPIO_HIGH )
    {
    ROBOT.nav.setColor (eColor::PREF);
    ROBOT.hmi.ledRGB.set(YELLOW, ON);
    LOG(INFO, "User has selected PREF (Yellow) color");
    }
  else
    {
    ROBOT.nav.setColor (eColor::SYM);
    ROBOT.hmi.ledRGB.set(BLUE, ON);
    LOG(INFO, "User has selected SYM (Blue) color");
    }

  //Read strat config
  strategyId = (ROBOT.hmi.user1.read() << 1) + (ROBOT.hmi.user2.read() );
  ardAssert(strategies[strategyId].functor != 0, "Selected strategy functor is null.");
  LOG(INFO, "User has selected strategy " + strategies[strategyId].name);

}
