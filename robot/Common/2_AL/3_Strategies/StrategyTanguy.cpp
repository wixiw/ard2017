#include "LinearStrategies.h"

using namespace ard;

void ard::Strategy_Tanguy (void* robotOpaque)
{
    Robot2017& robot = *reinterpret_cast<Robot2017*>(robotOpaque);
<<<<<<< HEAD
=======

	LOG_INFO("Strategy_Tanguy.");
>>>>>>> 47c8f5df3f245d1a6d1a376ebf746dc9ee2e0a2c

    LOG_INFO("Strategy_Tanguy.");

    robot.nav.setPosition(640,730,0);

<<<<<<< HEAD
//  robot.nav.goToCap(x_target + 260*cos(radians(theta)), y_target + 260*sin(radians(theta)), theta + 180, eDir_FORWARD);
=======
//	DispenserMonocolor lsaTopDisp(robot);
//	LOG_INFO("Go to Own Dispenser_Monocolor Rush entry point");
////	robot.nav.goToCap(lsaTopDisp.getEntryPoint());
//	robot.nav.goToCap(350, 730, 0, eDir_BACKWARD);
//	robot.nav.wait();
//	//lsaTopDisp.execute(/*get all cylinders*/);
>>>>>>> 47c8f5df3f245d1a6d1a376ebf746dc9ee2e0a2c

//    DispenserMonocolor lsaTopDisp(robot);
    LOG_INFO("Go to Own Dispenser_Monocolor Rush entry point");
//  robot.nav.goToCap(lsaTopDisp.getEntryPoint());
    robot.nav.goToCap(350, 730, 0, eDir_BACKWARD);
    robot.nav.wait();
//    lsaTopDisp.execute(/*get all cylinders*/);

    LOG_INFO("Avoiding D");
    robot.nav.goTo(350, 300 + 260*sin(radians(225)),  eDir_BACKWARD);
    robot.nav.wait();

<<<<<<< HEAD
    LOG_INFO("Goto B");
    robot.nav.goToCap(1200 + 260*cos(radians(225)), 300 + 260*sin(radians(225)), 225 + 180, eDir_FORWARD);
    robot.nav.wait();
=======
	LOG_INFO("Get B");
	ArdOs::sleepMs(1000);
	robot.stratInfo.informTaken_B();
>>>>>>> 47c8f5df3f245d1a6d1a376ebf746dc9ee2e0a2c

    LOG_INFO("Approach B");
    robot.nav.goTo(1200, 300, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Get B");
    ArdOs::sleepMs(1000);
    robot.stratInfo.informTaken_B();

    LOG_INFO("Face Recal top Y");
    robot.nav.turnTo(90);
    robot.nav.wait();

    LOG_INFO("Recal Y");
    robot.nav.goTo(1200, 470, eDir_FORWARD);
    robot.nav.wait();
    ArdOs::sleepMs(1000);

<<<<<<< HEAD
//  LOG_INFO("Recal en Y");
//  robot.nav.goTo(1140, 500, eDir_FORWARD);
//  robot.nav.wait();
=======
	LOG_INFO("Get F");
    while(!robot.nav.targetReached())
    {
        ArdOs::sleepMs(1000);
        robot.stratInfo.informTaken_F();
    }
>>>>>>> 47c8f5df3f245d1a6d1a376ebf746dc9ee2e0a2c

    LOG_INFO("Going Bwd");
    robot.nav.goToCap(1200, 370, 225, eDir_BACKWARD);
    robot.nav.wait();

    LOG_INFO("Going Recal top X");
    robot.nav.goToCap(1300, 470, 180, eDir_BACKWARD);
    robot.nav.wait();

<<<<<<< HEAD
    LOG_INFO("Recal top X");
    robot.nav.goTo(1470, 470, eDir_BACKWARD);
    robot.nav.wait();
    ArdOs::sleepMs(1000);

    LOG_INFO("Going Exit point");
    robot.nav.goTo(1300, 470, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Avoiding E while going F");
    robot.nav.goTo(800, 0, eDir_FORWARD);
    robot.nav.wait();
=======
	LOG_INFO("Poo F on table");
    while(!robot.nav.targetReached())
    {
        ArdOs::sleepMs(500);
        robot.stratInfo.informPooed_OnTable(1);
    }
>>>>>>> 47c8f5df3f245d1a6d1a376ebf746dc9ee2e0a2c

    LOG_INFO("Approach F");
    robot.nav.goToCap(775, -225,  -135, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Going Fwd");
    robot.nav.goForward(65);
    robot.nav.wait();

<<<<<<< HEAD
    LOG_INFO("Get F");
    ArdOs::sleepMs(1000);
    robot.stratInfo.informTaken_F();
=======
	LOG_INFO("Get C");
	ArdOs::sleepMs(1000);
	robot.stratInfo.informTaken_C();
>>>>>>> 47c8f5df3f245d1a6d1a376ebf746dc9ee2e0a2c

    LOG_INFO("Going Bwd");
    robot.nav.goForward(-65);
    robot.nav.wait();

    LOG_INFO("Go between D & E");
    robot.nav.goTo(750, -50,  eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Poo F on table");
    ArdOs::sleepMs(500);
    robot.stratInfo.informPooed_OnTable(1);

    LOG_INFO("Goto C");
    robot.nav.goToCap(790 + 260*cos(radians(60)), -690 + 260*sin(radians(60)), 60 + 180, eDir_BACKWARD);
    robot.nav.wait();

    LOG_INFO("Approach C");
    robot.nav.goTo(790, -690, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Get C");
    ArdOs::sleepMs(1000);
    robot.stratInfo.informTaken_C();

    LOG_INFO("Going Recal bottom X");
    robot.nav.turnTo(90);
    robot.nav.wait();

    LOG_INFO("Recal bottom X");
    robot.nav.goTo(790, -980, eDir_BACKWARD);
    robot.nav.wait();
    ArdOs::sleepMs(1000);

    LOG_INFO("Going Fwd");
    robot.nav.goForward(65);
    robot.nav.wait();

    LOG_INFO("Goto 4");
    robot.nav.goToCap(760, -250,  -135, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Approach 4");
    robot.nav.goForward(50);
    robot.nav.wait();

    LOG_INFO("Poo x6 in 4");
    for(int i = 6 ; 0 < i ; i--)
    {
        ArdOs::sleepMs(500);
        robot.stratInfo.informPooed_4(1);
    }

<<<<<<< HEAD
//  LOG_INFO("Avoiding obstacles");
//  robot.nav.goTo(700, -300, eDir_FORWARD);
//  robot.nav.wait();
//  robot.stratInfo.informPushedAway_Container();
=======
//	LOG_INFO("Avoiding obstacles");
//	robot.nav.goTo(700, -300, eDir_FORWARD);
//	robot.nav.wait();
//	robot.stratInfo.informPushedAway_Container();
>>>>>>> 47c8f5df3f245d1a6d1a376ebf746dc9ee2e0a2c

    LOG_INFO("Leave 4");
    robot.nav.goForward(-50);
//  robot.nav.goToCap(750, -350, 0,  eDir_BACKWARD);
    robot.nav.wait();

    LOG_INFO("Goto G");
    robot.nav.goToCap(1170, -350, 0, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Approach G");
    robot.nav.goForward(100);
    robot.nav.wait();

    LOG_INFO("Get 4 Cylinders");
    for(int i = 4 ; 0 < i ; i--)
    {
        ArdOs::sleepMs(500);
        robot.stratInfo.informWithdraw_G(1);
    }

    LOG_INFO("Leave G");
    robot.nav.goForward(-100);
    robot.nav.wait();

    LOG_INFO("Get E");
    robot.nav.goTo(1000, -100, eDir_FORWARD);
    robot.nav.wait();
    robot.stratInfo.informTaken_E();

<<<<<<< HEAD
    LOG_INFO("Get dropped F");
    robot.nav.goTo(750, 150,  eDir_FORWARD);
    robot.nav.wait();
//  robot.stratInfo.informTaken_F();

    LOG_INFO("Get D");
    robot.nav.goTo(500, 400, eDir_FORWARD);
    robot.nav.wait();
    robot.stratInfo.informTaken_D();
=======
	LOG_INFO("Get E");
	robot.nav.goTo(1000, -100, eDir_FORWARD);
	robot.nav.wait();
	robot.stratInfo.informTaken_E();

	LOG_INFO("Get D");
	robot.nav.goTo(500, 400, eDir_FORWARD);
	robot.nav.wait();
	robot.stratInfo.informTaken_D();
>>>>>>> 47c8f5df3f245d1a6d1a376ebf746dc9ee2e0a2c

    LOG_INFO("Goto 6");
    robot.nav.goTo(550, 550, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Poo x6");
    for(int i = 6 ; 0 < i ; i--)
    {
        ArdOs::sleepMs(500);
        robot.stratInfo.informPooed_6(1);
    }

    LOG_INFO("Finished.");

    while(1){}
}
