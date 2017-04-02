/* 
* StrategyAlpha.cpp
*
* Created: 08/12/2016 00:20:33
* Author: wix
*/

#include "Strategies.h"
#include "../Robot2017.h"

#ifdef BUILD_STRATEGY

using namespace ard;

void ard::Strategy_Alpha (Robot2017& robot)
{
	LOG_INFO("STRAT : Strategy_Alpha.");

	//robot.nav.setPosition(610,820,-90);

	while(1)
	{
        LOG_INFO("STRAT : Move order 1 sent");
        robot.nav.goToCap(0, 0, 0, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 2 sent");
        robot.nav.goToCap(0, 0, 90, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 3 sent");
        robot.nav.goToCap(300, 0, 90, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 4 sent");
        robot.nav.goTo(0, 0, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 5 sent");
        robot.nav.goTo(0, 300, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 6 sent");
        robot.nav.goTo(300, 300, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 7 sent");
        robot.nav.goTo(300, 0, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 8 sent");
        robot.nav.goTo(0, 0, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 9 sent");
        robot.nav.goTo(300, 300, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 10 sent");
        robot.nav.goTo(0, 0, eDir_FORWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 11 sent");
        robot.nav.goToCap(300, 300, 0, eDir_BACKWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 12 sent");
        robot.nav.goTo(0, 0, eDir_BACKWARD);
        robot.nav.wait();

        LOG_INFO("STRAT : Move order 13 sent");
        robot.nav.goToCap(0, 0, eDir_BACKWARD);
        robot.nav.wait();

        ArdOs::sleepMs(2000);
	}

    LOG_INFO("STRAT : Finished.");

	robot.dieMotherFucker();
}

#endif
