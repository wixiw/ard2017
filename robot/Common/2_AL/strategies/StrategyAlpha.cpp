/* 
* StrategyAlpha.cpp
*
* Created: 08/12/2016 00:20:33
* Author: wix
*/


#include "StrategyAlpha.h"
#include "../Robot2017.h"

using namespace ard;

void
ard::Strategy_Alpha ()
{
	LOG_INFO("STRAT : Strategy_Alpha.");

	//Set robot to default position
	while(1)
	{
        ROBOT.nav.setPosition(610,820,-90);
//        ROBOT.nav.goTo(300, 0, eDir_FORWARD);
//        LOG_INFO("STRAT : Move order sent, waiting...");
//        ROBOT.nav.wait();
        LOG_INFO("STRAT : Return to start pos, waiting...");
//        ROBOT.nav.goTo(0, 0, eDir_FORWARD);
//        ROBOT.nav.wait();
        LOG_INFO("STRAT : Finished.");
        ArdOs::sleepMs(3000);
	}

	ROBOT.dieMotherFucker();
}
