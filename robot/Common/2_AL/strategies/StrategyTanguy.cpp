#include "Strategies.h"
#include "../Robot2017.h"

using namespace ard;

void ard::Strategy_Tanguy (Robot2017& robot)
{
	LOG_INFO("Strategy_Tanguy.");

	robot.nav.setPosition(610,820,180);

	DispenserMonocolor lsaTopDisp(robot);
	LOG_INFO("Go to Dispenser_Monocolor entry point");
	robot.nav.goToCap(lsaTopDisp.getEntryPoint());
	robot.nav.wait();
	lsaTopDisp.execute(/*get all cylinders*/);

	LOG_INFO("Approach MidPoo zone");
	robot.nav.goTo(0, 0, eDir_FORWARD);
	robot.nav.goTo(0, - 30, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Poo x4 in Middle Center area");
    for(int i = 4 ; 0 < i ; i--)
    {
        ArdOs::sleepMs(500);
        robot.strategy.informPooed_MiddleCenter(1);
    }

	LOG_INFO("Leave MidPoo zone");
	robot.nav.goTo(0, 0,  eDir_BACKWARD);
	robot.nav.wait();

	LOG_INFO("Avoiding obstacles");
	robot.nav.goTo(700, -300, eDir_FORWARD);
	robot.nav.wait();
	robot.strategy.informPushedAway_Container();

	LOG_INFO("Goto Y_B");
	robot.nav.goToCap(700, -800, -90, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Get Y_B");
	ArdOs::sleepMs(1000);
	robot.strategy.informTaken_Crater();

	LOG_INFO("Goto Y_R");
	//        robot.nav.goTo(1300-robot.conf.xav(), 400-robot.conf.yside(), eDir_FORWARD);
	robot.nav.goTo(1200, 300, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Get Y_R");
	ArdOs::sleepMs(1000);
	robot.strategy.informTaken_Corner();

	LOG_INFO("Approach Y_Poo zone");
	robot.nav.goToCap(670, -335, -135,eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Poo x2");
    for(int i = 2 ; 0 < i ; i--)
    {
        ArdOs::sleepMs(500);
        robot.strategy.informPooed_MiddleOwn(1);
    }

	LOG_INFO("Leave Y_Poo zone");
	robot.nav.goToCap(750, -350, 0,  eDir_BACKWARD);
	robot.nav.wait();

	LOG_INFO("Approach Y_Distributor");
	robot.nav.goTo(1360, -350, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Get 4 Cylinders");
    for(int i = 4 ; 0 < i ; i--)
    {
        ArdOs::sleepMs(500);
        robot.strategy.informWithdraw_BicolorDispenser(1);
    }

	LOG_INFO("Leave Y_Distributor");
	robot.nav.goTo(1200, -350, eDir_BACKWARD);
	robot.nav.wait();

	LOG_INFO("Get W_Mid");
	robot.nav.goTo(1000, -100, eDir_FORWARD);
	robot.nav.wait();
	robot.strategy.informTaken_Center();

	LOG_INFO("Get W_Top");
	robot.nav.goTo(500, 400, eDir_FORWARD);
	robot.nav.wait();
	robot.strategy.informTaken_Start();

	LOG_INFO("Go to start area");
	robot.nav.goTo(550, 650, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Poo x6");
    for(int i = 6 ; 0 < i ; i--)
    {
        ArdOs::sleepMs(500);
        robot.strategy.informPooed_Start(1);
    }

    LOG_INFO("Finished.");

	robot.dieMotherFucker();
}
