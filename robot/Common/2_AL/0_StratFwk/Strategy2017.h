/*
 * Strategy2017.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_0_STRATFWK_STRATEGY2017_H_
#define ROBOT_COMMON_2_AL_0_STRATFWK_STRATEGY2017_H_

#include "RSP.h"
#ifdef BUILD_STRATEGY
#include "0_StratFwk/StratInterfaces.h"
#include "generated/sc_types.h"

namespace ard
{
    //forward declare
    class Robot2017;

    /**
     * This class contains a standard robot actuators/sensors interface
     * so that each build strategy doesn't have to redefine its own
     */
    class Strategy2017: public IStrategy
    {
    public:
        Strategy2017(Robot2017* robot, String const& name);

    protected:
        Robot2017& robot;
    };

} /* namespace ard */
#endif

#define STRAT_2017_API_ITF()\
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
void swallow(sc_integer nbCylinders);\
void retractArms();\
void withdraw(sc_integer nbCylinders);\
void poo(sc_integer nbCylinders);\
void disableActuators();


#define STRAT_2017_API_IMPL(myclass)\
void myclass::beep(sc_integer nb)\
{\
    robot.buzzer().bip(nb);\
}\
\
void myclass::led1(sc_integer blink)\
{\
    robot.hmi.led1.set((eLedState)(blink));\
}\
\
void myclass::led2(sc_integer blink)\
{\
    robot.hmi.led2.set((eLedState)(blink));\
}\
\
void myclass::led3(sc_integer blink)\
{\
    robot.hmi.led3.set((eLedState)(blink));\
}\
\
void myclass::led4(sc_integer blink)\
{\
    robot.hmi.led4.set((eLedState)(blink));\
}\
\
void myclass::ledRGB(sc_integer color, sc_integer blink)\
{\
    robot.hmi.ledRGB.set((eRgb)(color), eLedState(blink));\
}\
\
bool myclass::isStartPlugged()\
{\
    return robot.isStartPlugged();\
}\
\
bool myclass::isColorSwitchOnPrefered()\
{\
    return robot.isColorSwitchOnPrefered();\
}\
\
bool myclass::isUser1SwitchOn()\
{\
    return robot.hmi.user1.read();\
}\
\
bool myclass::isUser2SwitchOn()\
{\
    return robot.hmi.user2.read();\
}\
\
void myclass::enableAvoidance(sc_boolean on)\
{\
    robot.nav.enableAvoidance(on);\
}\
\
void myclass::setPosition(sc_real x, sc_real y, sc_real h)\
{\
    robot.nav.setPosition(x, y, h);\
}\
\
void myclass::setSpeedAcc(sc_integer vMax, sc_integer vMaxsTurn, sc_integer accMax, sc_integer accMaxTurn)\
{\
    robot.nav.setSpeedAcc(vMax, vMaxsTurn, accMax, accMaxTurn);\
}\
\
void myclass::goTo_ID(sc_real x, sc_real y, sc_integer sens)\
{\
    robot.nav.goTo(x, y, (eDir)(sens));\
}\
\
void myclass::goToCap(sc_real x, sc_real y, sc_real h, sc_integer sens)\
{\
    robot.nav.goToCap(x, y, h, (eDir)(sens));\
}\
\
void myclass::goForward(sc_real distanceMm)\
{\
    robot.nav.goForward(distanceMm);\
}\
\
void myclass::turnDelta(sc_real angle)\
{\
    robot.nav.turnDelta(angle);\
}\
\
void myclass::turnTo(sc_real angle)\
{\
    robot.nav.turnTo(angle);\
}\
\
void myclass::faceTo(sc_real x, sc_real y)\
{\
    robot.nav.faceTo(x, y);\
}\
\
void myclass::recalFace(sc_integer border)\
{\
    robot.nav.recalFace((eTableBorder)(border));\
}\
\
void myclass::recalRear(sc_integer border)\
{\
    robot.nav.recalRear((eTableBorder)(border));\
}\
\
void myclass::stopMoving()\
{\
    robot.nav.stopMoving();\
}\
\
sc_boolean myclass::targetReached()\
{\
    return robot.nav.targetReached();\
}\
\
void ard::myclass::swallow(sc_integer nbCylinders)\
{\
    robot.actuators.swallow(nbCylinders);\
}\
\
void ard::myclass::retractArms()\
{\
    robot.actuators.retractArms();\
}\
\
void ard::myclass::withdraw(sc_integer nbCylinders)\
{\
    robot.actuators.withdraw(nbCylinders);\
}\
\
void ard::myclass::poo(sc_integer nbCylinders)\
{\
    robot.actuators.poo(nbCylinders);\
}\
void ard::myclass::disableActuators()\
{\
    robot.actuators.disableAll();\
}


#endif /* ROBOT_COMMON_2_AL_0_STRATFWK_STRATEGY2017_H_ */
