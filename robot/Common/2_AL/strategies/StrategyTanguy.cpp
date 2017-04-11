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

	robot.nav.setPosition(610,820,180);

	LOG_INFO("STRAT : Go to Distributor Entry point");
	robot.nav.goToCap(350, 750, 90, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Approach Top_Distributor");
	robot.nav.goTo(350, 860, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Get 4 Cylinders");
	ArdOs::sleepMs(12000);

	LOG_INFO("STRAT : Leave Top_Distributor");
	robot.nav.goToCap(350, 750, -115, eDir_BACKWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Go to Middle");
	robot.nav.goToCap(0, 0, -90, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Approach MidPoo zone");
	robot.nav.goTo(0, -100, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Poo x4");
	ArdOs::sleepMs(12000);

	LOG_INFO("STRAT : Leave MidPoo zone");
	robot.nav.goToCap(0, 0, -25,  eDir_BACKWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Avoiding obstacles");
	robot.nav.goTo(700, -300, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Goto Y_B");
	robot.nav.goToCap(700, -800, -90, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Get Y_B");
	ArdOs::sleepMs(2000);

	LOG_INFO("STRAT : Goto Y_R");
	//        robot.nav.goTo(1300-robot.conf.xav(), 400-robot.conf.yside(), eDir_FORWARD);
	robot.nav.goTo(1200, 300, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Get Y_R");
	ArdOs::sleepMs(2000);

	LOG_INFO("STRAT : Approach Y_Poo zone");
	robot.nav.goToCap(670, -335, -135,eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Poo x2");
	ArdOs::sleepMs(12000);

	LOG_INFO("STRAT : Leave Y_Poo zone");
	robot.nav.goToCap(750, -350, 0,  eDir_BACKWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Approach Y_Distributor");
	robot.nav.goTo(1360, -350, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Get 4 Cylinders");
	ArdOs::sleepMs(12000);

	LOG_INFO("STRAT : Leave Y_Distributor");
	robot.nav.goToCap(1300, -350, 140, eDir_BACKWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Get W_Mid");
	robot.nav.goTo(1000, -100, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Get W_Top");
	robot.nav.goToCap(500, 400, 80, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Get W_Mid");
	robot.nav.goTo(550, 650, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("STRAT : Poo x6");
	ArdOs::sleepMs(15000);


    LOG_INFO("STRAT : Finished.");

	robot.dieMotherFucker();
}
