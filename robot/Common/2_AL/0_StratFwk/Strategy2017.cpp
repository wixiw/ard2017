/*
 * Strategy2017.cpp
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#include "Strategy2017.h"
#ifdef BUILD_STRATEGY
#include "Robot2017.h"

using namespace ard;

Strategy2017::Strategy2017(Robot2017* _robot):
        robot(*_robot)
{
    ASSERT_TEXT(_robot, "You did not attached a robot.");
}

void Strategy2017::beep(sc_integer nb)
{
    robot.buzzer().bip(nb);
}

void Strategy2017::led1(sc_integer blink)
{
    robot.hmi.led1.set((eLedState)(blink));
}

void Strategy2017::led2(sc_integer blink)
{
    robot.hmi.led2.set((eLedState)(blink));
}

void Strategy2017::led3(sc_integer blink)
{
    robot.hmi.led3.set((eLedState)(blink));
}

void Strategy2017::led4(sc_integer blink)
{
    robot.hmi.led4.set((eLedState)(blink));
}

void Strategy2017::ledRGB(sc_integer color, sc_integer blink)
{
    robot.hmi.ledRGB.set((eRgb)(color), eLedState(blink));
}

void Strategy2017::enableAvoidance(bool on)
{
    robot.nav.enableAvoidance(on);
}

void Strategy2017::setPosition(float x, float y, float h)
{
    robot.nav.setPosition(x, y, h);
}

void Strategy2017::setSpeedAcc(sc_integer vMax, sc_integer vMaxsTurn, sc_integer accMax, sc_integer accMaxTurn)
{
    robot.nav.setSpeedAcc(vMax, vMaxsTurn, accMax, accMaxTurn);
}

void Strategy2017::goTo(float x, float y, eDir sens)
{
    robot.nav.goTo(x, y, sens);
}

void Strategy2017::goToCap(float x, float y, float h, eDir sens)
{
    robot.nav.goToCap(x, y, h, sens);
}

void Strategy2017::goForward(float distanceMm)
{
    robot.nav.goForward(distanceMm);
}

void Strategy2017::turnDelta(float angle)
{
    robot.nav.turnDelta(angle);
}

void Strategy2017::turnTo(float angle)
{
    robot.nav.turnTo(angle);
}

void Strategy2017::faceTo(float x, float y)
{
    robot.nav.faceTo(x, y);
}

void Strategy2017::recalFace(sc_integer border)
{
    robot.nav.recalFace((eTableBorder)(border));
}

void Strategy2017::recalRear(sc_integer border)
{
    robot.nav.recalRear((eTableBorder)(border));
}

void Strategy2017::stopMoving()
{
    robot.nav.stopMoving();
}

bool Strategy2017::targetReached() const
{
    return robot.nav.targetReached();
}

void ard::Strategy2017::swallow(sc_integer nbCylinders)
{
    robot.actuators.swallow(nbCylinders);
}

void ard::Strategy2017::retractArms()
{
    robot.actuators.retractArms();
}

void ard::Strategy2017::withdraw(sc_integer nbCylinders)
{
    robot.actuators.withdraw(nbCylinders);
}

void ard::Strategy2017::poo(sc_integer nbCylinders)
{
    robot.actuators.poo(nbCylinders);
}

#endif
