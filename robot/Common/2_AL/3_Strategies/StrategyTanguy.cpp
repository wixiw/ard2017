#include "LinearStrategies.h"

using namespace ard;

void ard::Strategy_Tanguy (void* robotOpaque)
{
    Robot2017& robot = *reinterpret_cast<Robot2017*>(robotOpaque);

	LOG_INFO("Strategy_Tanguy.");

	robot.nav.setPosition(640,730,0);

//	robot.nav.goToCap(x_target + 260*cos(radians(theta)), y_target + 260*sin(radians(theta)), theta + 180, eDir_FORWARD);

//	DispenserMonocolor lsaTopDisp(robot);
//	LOG_INFO("Go to Own Dispenser_Monocolor Rush entry point");
////	robot.nav.goToCap(lsaTopDisp.getEntryPoint());
//	robot.nav.goToCap(350, 730, 0, eDir_BACKWARD);
//	robot.nav.wait();
//	//lsaTopDisp.execute(/*get all cylinders*/);

	LOG_INFO("Avoiding D while going B");
	robot.nav.goTo(350, -100,  eDir_BACKWARD);
	robot.nav.wait();

	LOG_INFO("Goto B");
	robot.nav.goTo(1300, 400, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Get B");
	ArdOs::sleepMs(1000);
	robot.stratInfo.informTaken_B();

	LOG_INFO("Recal en X");
	robot.nav.goTo(1355, 460, eDir_FORWARD);
    while(!robot.nav.targetReached())
    {
        ArdOs::sleepMs(1000);
    }

	LOG_INFO("Recal en Y");
	robot.nav.goTo(1140, 588, eDir_BACKWARD);
    while(!robot.nav.targetReached())
    {
        ArdOs::sleepMs(1000);
    }

	LOG_INFO("Going Fwd");
	robot.nav.goForward(100);
	robot.nav.wait();

	LOG_INFO("Avoiding E while going F");
	robot.nav.goTo(600, -400,  eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Get F");
    while(!robot.nav.targetReached())
    {
        ArdOs::sleepMs(1000);
        robot.stratInfo.informTaken_F();
    }

	LOG_INFO("Going Bwd");
	robot.nav.goForward(-100);
	robot.nav.wait();

	LOG_INFO("Go between D & E");
	robot.nav.goTo(750, 150,  eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Poo F on table");
    while(!robot.nav.targetReached())
    {
        ArdOs::sleepMs(500);
        robot.stratInfo.informPooed_OnTable(1);
    }

	LOG_INFO("Goto C");
	robot.nav.goToCap(700 + 260*cos(radians(70)), -850 + 260*sin(radians(70)), 70 + 180, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Approach C");
//	Remplacer par un goForward(); quand il sera code
	robot.nav.goToCap(700 + 110*cos(radians(70)), -850 + 110*sin(radians(70)), 70 + 180, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Get C");
	ArdOs::sleepMs(1000);
	robot.stratInfo.informTaken_C();

	LOG_INFO("Goto 4");
	robot.nav.goTo(800, -150,eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Approach 4");
	robot.nav.goToCap(680, -320, -135,eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Poo x6 in 4");
    for(int i = 6 ; 0 < i ; i--)
    {
        ArdOs::sleepMs(500);
        robot.stratInfo.informPooed_4(1);
    }

//	LOG_INFO("Avoiding obstacles");
//	robot.nav.goTo(700, -300, eDir_FORWARD);
//	robot.nav.wait();
//	robot.stratInfo.informPushedAway_Container();

	LOG_INFO("Leave 4");
	robot.nav.goToCap(750, -350, 0,  eDir_BACKWARD);
	robot.nav.wait();

	LOG_INFO("Approach G");
	robot.nav.goTo(1360, -350, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Get 4 Cylinders");
    for(int i = 4 ; 0 < i ; i--)
    {
        ArdOs::sleepMs(500);
        robot.stratInfo.informWithdraw_G(1);
    }

	LOG_INFO("Leave G");
	robot.nav.goTo(1200, -350, eDir_BACKWARD);
	robot.nav.wait();

	LOG_INFO("Get E");
	robot.nav.goTo(1000, -100, eDir_FORWARD);
	robot.nav.wait();
	robot.stratInfo.informTaken_E();

	LOG_INFO("Get D");
	robot.nav.goTo(500, 400, eDir_FORWARD);
	robot.nav.wait();
	robot.stratInfo.informTaken_D();

	LOG_INFO("Goto 6");
	robot.nav.goTo(550, 650, eDir_FORWARD);
	robot.nav.wait();

	LOG_INFO("Poo x6");
    for(int i = 6 ; 0 < i ; i--)
    {
        ArdOs::sleepMs(500);
        robot.stratInfo.informPooed_6(1);
    }

    LOG_INFO("Finished.");

	robot.dieMotherFucker();
}
