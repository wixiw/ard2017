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

	ROBOT.nav.setPosition(610,820,-90);

    LOG_INFO("STRAT : Move order sent");
    ROBOT.nav.goTo(300, 0, eDir_FORWARD);
    LOG_INFO("STRAT : Return to start pos");
    ROBOT.nav.goTo(0, 0, eDir_FORWARD);
    LOG_INFO("waiting...");
    ROBOT.nav.wait();
    LOG_INFO("STRAT : Finished.");
    ArdOs::sleepMs(3000);

	ROBOT.dieMotherFucker();
}
