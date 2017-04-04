/* 
* StrategyTanguy.cpp
*
* Created: 08/12/2016 00:20:33
* Author: tanguy
*/


#include "Strategies.h"
#include "../Robot2017.h"

using namespace ard;

void
ard::Strategy_Tanguy (Robot2017& robot)
{
	LOG_INFO("STRAT : Strategy_Tanguy.");

	ROBOT.nav.setPosition(610,820,-90);

	while(1)
	{
        LOG_INFO("STRAT : Move order 1 sent");
        ROBOT.nav.goToCap(0, 0, 0, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 5 sent");
        ROBOT.nav.goTo(0, 300, eDir_FORWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 11 sent");
        ROBOT.nav.goToCap(300, 300, 0, eDir_BACKWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 12 sent");
        ROBOT.nav.goTo(0, 0, eDir_BACKWARD);
        ROBOT.nav.wait();

        LOG_INFO("STRAT : Move order 13 sent");
        ROBOT.nav.goToCap(0, 0, eDir_BACKWARD);
        ROBOT.nav.wait();

        ArdOs::sleepMs(2000);
	}

    LOG_INFO("STRAT : Finished.");

	ROBOT.dieMotherFucker();
}
