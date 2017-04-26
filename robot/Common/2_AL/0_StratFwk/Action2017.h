/*
 * Action2017.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_
#define ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_

#include "RSP.h"
#ifdef BUILD_STRATEGY
#include "generated/sc_types.h"

namespace ard
{
    //forward declare
    class Robot2017;

    class Action2017: public PolledObject
    {
    public:
        Action2017(Robot2017* robot, String const& name);
    protected:
        Robot2017& robot;
    };

#define ACTION_2017_API_ITF()\
void beep(sc_integer nb);\
void led1(sc_integer blink);\
void led2(sc_integer blink);\
void led3(sc_integer blink);\
void led4(sc_integer blink);\
void ledRGB(sc_integer color, sc_integer blink);\
bool isStartPlugged();\
bool isColorSwitchOnPrefered();\
bool isUser1SwitchOn();\
bool isUser2SwitchOn();\
void logDebug(sc_string msg);\
void logInfo(sc_string msg);\
void logError(sc_string msg);\
void enableAvoidance(sc_boolean on);\
void setPosition(sc_real x/*mm*/, sc_real y/*mm*/, sc_real h/*deg*/);\
void setSpeedAcc(sc_integer vMax /*mm/s*/, sc_integer vMaxsTurn /*°/s*/,\
        sc_integer accMax /*mm/s2*/, sc_integer accMaxTurn /*°/s2*/);\
void goTo_ID(sc_real x /*mm*/, sc_real y/*mm*/, sc_integer sens);\
void goToCap(sc_real x/*mm*/, sc_real y/*mm*/, sc_real h/*°*/, sc_integer sens);\
void goForward(sc_real distanceMm);\
void turnDelta(sc_real angle/*deg*/);\
void turnTo(sc_real angle/*deg*/);\
void faceTo(sc_real x/*mm*/, sc_real y/*mm*/);\
void recalFace(sc_integer border);\
void recalRear(sc_integer border);\
void stopMoving();\
sc_boolean targetReached();\
void swallow(sc_boolean on);\
void turnWheels(sc_integer on);\
void lifter(sc_boolean up);\
void arms(sc_integer left, sc_integer right);\
\
void informWithdraw_A(sc_integer nb);\
void informWithdraw_G(sc_integer nb);\
void informWithdraw_Opp_G(sc_integer nb);\
void informPooed_3(sc_integer nb);\
void informPooed_4(sc_integer nb);\
void informPooed_2(sc_integer nb);\
void informPooed_5(sc_integer nb);\
void informPooed_Opp_5(sc_integer nb);\
void informPooed_6(sc_integer nb);\
void informPooed_OnTable(sc_integer nb);\
void informTaken_D();\
void informTaken_F();\
void informTaken_E();\
void informTaken_B();\
void informTaken_C();\
void informTaken_Opp_D();\
void informTaken_Opp_F();\
void informTaken_Opp_E();\
void informPushedAway_D();\
void informPushedAway_F();\
void informPushedAway_E();\
void informPushedAway_B();\
void informPushedAway_C();\
void informPushedAway_Opp_D();\
void informPushedAway_Opp_F();\
void informPushedAway_Opp_E();

} /* namespace ard */
#endif



#define ACTION_2017_API_IMPL(myclass)\
void myclass::beep(sc_integer nb)\
{\
    robot.buzzer().bip(nb);\
}\
void myclass::led1(sc_integer blink)\
{\
    robot.hmi.led1.set((eLedState)(blink));\
}\
void myclass::led2(sc_integer blink)\
{\
    robot.hmi.led2.set((eLedState)(blink));\
}\
void myclass::led3(sc_integer blink)\
{\
    robot.hmi.led3.set((eLedState)(blink));\
}\
void myclass::led4(sc_integer blink)\
{\
    robot.hmi.led4.set((eLedState)(blink));\
}\
void myclass::ledRGB(sc_integer color, sc_integer blink)\
{\
    robot.hmi.ledRGB.set((eRgb)(color), eLedState(blink));\
}\
bool myclass::isStartPlugged()\
{\
    return robot.isStartPlugged();\
}\
bool myclass::isColorSwitchOnPrefered()\
{\
    return robot.isColorSwitchOnPrefered();\
}\
bool myclass::isUser1SwitchOn()\
{\
    return robot.hmi.user1.read();\
}\
bool myclass::isUser2SwitchOn()\
{\
    return robot.hmi.user2.read();\
}\
void myclass::logDebug(sc_string msg)\
{\
    LOG_DEBUG(msg);\
}\
void myclass::logInfo(sc_string msg)\
{\
    LOG_INFO(msg);\
}\
void myclass::logError(sc_string msg)\
{\
    LOG_ERROR(msg);\
}\
void myclass::enableAvoidance(sc_boolean on)\
{\
    robot.nav.enableAvoidance(on);\
}\
void myclass::setPosition(sc_real x, sc_real y, sc_real h)\
{\
    robot.nav.setPosition(x, y, h);\
}\
void myclass::setSpeedAcc(sc_integer vMax, sc_integer vMaxsTurn, sc_integer accMax, sc_integer accMaxTurn)\
{\
    robot.nav.setSpeedAcc(vMax, vMaxsTurn, accMax, accMaxTurn);\
}\
void myclass::goTo_ID(sc_real x, sc_real y, sc_integer sens)\
{\
    robot.nav.goTo(x, y, (eDir)(sens));\
}\
void myclass::goToCap(sc_real x, sc_real y, sc_real h, sc_integer sens)\
{\
    robot.nav.goToCap(x, y, h, (eDir)(sens));\
}\
void myclass::goForward(sc_real distanceMm)\
{\
    robot.nav.goForward(distanceMm);\
}\
void myclass::turnDelta(sc_real angle)\
{\
    robot.nav.turnDelta(angle);\
}\
void myclass::turnTo(sc_real angle)\
{\
    robot.nav.turnTo(angle);\
}\
void myclass::faceTo(sc_real x, sc_real y)\
{\
    robot.nav.faceTo(x, y);\
}\
void myclass::recalFace(sc_integer border)\
{\
    robot.nav.recalFace((eTableBorder)(border));\
}\
void myclass::recalRear(sc_integer border)\
{\
    robot.nav.recalRear((eTableBorder)(border));\
}\
void myclass::stopMoving()\
{\
    robot.nav.stopMoving();\
}\
sc_boolean myclass::targetReached()\
{\
    return robot.nav.targetReached();\
}\
void myclass::swallow(sc_boolean on)\
{\
    robot.actuators.swallow(on);\
}\
void myclass::turnWheels(sc_integer on)\
{\
    robot.actuators.turnWheels(on);\
}\
void myclass::lifter(sc_boolean up)\
{\
    robot.actuators.lifterCmd(up);\
}\
void myclass::arms(sc_integer left, sc_integer right)\
{\
    robot.actuators.servoLeftArm.write(left);\
    robot.actuators.servoRightArm.write(right);\
}\
void myclass::informWithdraw_A(sc_integer nb)\
{\
    robot.strategy.informWithdraw_A(nb);\
}\
void myclass::informWithdraw_G(sc_integer nb)\
{\
    robot.strategy.informWithdraw_G(nb);\
}\
void myclass::informWithdraw_Opp_G(sc_integer nb)\
{\
    robot.strategy.informWithdraw_Opp_G(nb);\
}\
void myclass::informPooed_3(sc_integer nb)\
{\
    robot.strategy.informPooed_3(nb);\
}\
void myclass::informPooed_4(sc_integer nb)\
{\
    robot.strategy.informPooed_4(nb);\
}\
void myclass::informPooed_2(sc_integer nb)\
{\
    robot.strategy.informPooed_2(nb);\
}\
void myclass::informPooed_5(sc_integer nb)\
{\
    robot.strategy.informPooed_5(nb);\
}\
void myclass::informPooed_Opp_5(sc_integer nb)\
{\
    robot.strategy.informPooed_Opp_5(nb);\
}\
void myclass::informPooed_6(sc_integer nb)\
{\
    robot.strategy.informPooed_6(nb);\
}\
void myclass::informPooed_OnTable(sc_integer nb)\
{\
    robot.strategy.informPooed_OnTable(nb);\
}\
void myclass::informTaken_D()\
{\
    robot.strategy.informTaken_D();\
}\
void myclass::informTaken_F()\
{\
    robot.strategy.informTaken_F();\
}\
void myclass::informTaken_E()\
{\
    robot.strategy.informTaken_E();\
}\
void myclass::informTaken_B()\
{\
    robot.strategy.informTaken_B();\
}\
void myclass::informTaken_C()\
{\
    robot.strategy.informTaken_C();\
}\
void myclass::informTaken_Opp_D()\
{\
    robot.strategy.informTaken_Opp_D();\
}\
void myclass::informTaken_Opp_F()\
{\
    robot.strategy.informTaken_Opp_F();\
}\
void myclass::informTaken_Opp_E()\
{\
    robot.strategy.informTaken_Opp_E();\
}\
void myclass::informPushedAway_D()\
{\
    robot.strategy.informPushedAway_D();\
}\
void myclass::informPushedAway_F()\
{\
    robot.strategy.informPushedAway_F();\
}\
void myclass::informPushedAway_E()\
{\
    robot.strategy.informPushedAway_E();\
}\
void myclass::informPushedAway_B()\
{\
    robot.strategy.informPushedAway_B();\
}\
void myclass::informPushedAway_C()\
{\
    robot.strategy.informPushedAway_C();\
}\
void myclass::informPushedAway_Opp_D()\
{\
    robot.strategy.informPushedAway_Opp_D();\
}\
void myclass::informPushedAway_Opp_F()\
{\
    robot.strategy.informPushedAway_Opp_F();\
}\
void myclass::informPushedAway_Opp_E()\
{\
    robot.strategy.informPushedAway_Opp_E();\
}\


#endif /* ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_ */
