#include "../Robot.h"
#include "Strategies.h"

using namespace ard;
    
void ard::Strategy_Tanguy (Robot2017& robot)
{
    LSA_Dispenser lsaTopDisp(robot, Monocolor);
    LSA_Dispenser lsaBotDisp(robot, Bicolor);

    LOG_INFO("Strategy_Tanguy.");

    robot.nav.setPosition(640,730,0);

    LOG_INFO("Go to Own Dispenser_Monocolor Rush entry point");
    robot.nav.goToCap(350, 730, 90, eDir_FORWARD);
    lsaTopDisp.startLSA();
    while( Success != lsaTopDisp.isFinished() )
    {
        lsaTopDisp.updateLSA(PERIOD_STRATEGY);
        ArdOs::sleepMs(PERIOD_STRATEGY);
    }

    LOG_INFO("Avoiding D");
    robot.nav.goTo(350, 300 + 260*sin(radians(225)),  eDir_BACKWARD);
    robot.nav.wait();

    LOG_INFO("Goto B");
    robot.nav.goToCap(1200 + 260*cos(radians(225)), 300 + 260*sin(radians(225)), 225 + 180, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Approach B");
    robot.nav.goTo(1200, 300, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Get B");
    ArdOs::sleepMs(1000);
    robot.stratInfo.informTaken_B();

    LOG_INFO("Recal Y");
    robot.nav.recalFace(eTableBorder_START_AREA_Y);
    robot.nav.wait();

//    LOG_INFO("Going Bwd");
//    robot.nav.goToCap(1200, 370, 225, eDir_BACKWARD);
//    robot.nav.wait();

    LOG_INFO("Going Recal top X");
    robot.nav.goToCap(1300, 470, 180, eDir_BACKWARD);
    robot.nav.wait();

    LOG_INFO("Recal top X");
    robot.nav.recalRear(eTableBorder_OWN_SIDE_X);
    robot.nav.wait();

//    LOG_INFO("Going Exit point");
//    robot.nav.goTo(1300, 470, eDir_FORWARD);
//    robot.nav.wait();

    LOG_INFO("Avoiding E while going F");
    robot.nav.goTo(800, 0, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Approach F");
    robot.nav.goToCap(775, -225,  -135, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Going Fwd");
    robot.nav.goForward(65);
    robot.nav.wait();

    LOG_INFO("Get F");
    ArdOs::sleepMs(1000);
    robot.stratInfo.informTaken_F();

    LOG_INFO("Going Bwd");
    robot.nav.goForward(-65);
    robot.nav.wait();

    LOG_INFO("Go between D & E");
    robot.nav.goTo(750, -50,  eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Poo F on table");
    ArdOs::sleepMs(500);
    robot.stratInfo.informPooed_OnTable(1);
    robot.nav.goForward(-40);//goBackward a bit to prevent to touch the cylinder during a rotation

    LOG_INFO("Goto C");
    robot.nav.goToCap(790 + 260*cos(radians(60)), -690 + 260*sin(radians(60)), 60 + 180, eDir_BACKWARD);
    robot.nav.wait();

    LOG_INFO("Approach C");
    robot.nav.goTo(790, -690, eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Get C");
    ArdOs::sleepMs(1000);
    robot.stratInfo.informTaken_C();

    LOG_INFO("Recal bottom X");
    robot.nav.recalRear(eTableBorder_BOT_Y);
    robot.nav.wait();

//    LOG_INFO("Going Fwd");
//    robot.nav.goForward(65);
//    robot.nav.wait();

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

//  LOG_INFO("Avoiding obstacles");
//  robot.nav.goTo(700, -300, eDir_FORWARD);
//  robot.nav.wait();
//  robot.stratInfo.informPushedAway_Container();

    LOG_INFO("Leave 4");
    robot.nav.goForward(-50);
//  robot.nav.goToCap(750, -350, 0,  eDir_BACKWARD);
    robot.nav.wait();

    LOG_INFO("Goto G");
    robot.nav.goToCap(lsaBotDisp.getEntryPoint(), eDir_FORWARD);
    robot.nav.wait();

    LOG_INFO("Get 4 Cylinders");
    lsaBotDisp.startLSA();
    while( Success != lsaBotDisp.isFinished() )
    {
        lsaBotDisp.updateLSA(PERIOD_STRATEGY);
        ArdOs::sleepMs(PERIOD_STRATEGY);
    }

    LOG_INFO("Leave G");
    robot.nav.goForward(-100);
    robot.nav.wait();

    LOG_INFO("Get E");
    robot.nav.goTo(1000, -100, eDir_FORWARD);
    robot.nav.wait();
    robot.stratInfo.informTaken_E();

    LOG_INFO("Get dropped F");
    robot.nav.goTo(750, 150,  eDir_FORWARD);
    robot.nav.wait();
//  robot.stratInfo.informTaken_F();

    LOG_INFO("Get D");
    robot.nav.goTo(500, 400, eDir_FORWARD);
    robot.nav.wait();
    robot.stratInfo.informTaken_D();

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

    while(1){};
}
