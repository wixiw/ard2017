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
        Strategy2017(Robot2017* robot);

        /**
         * HMI
         */
        void beep(sc_integer nb);
        void led1(sc_integer blink);
        void led2(sc_integer blink);
        void led3(sc_integer blink);
        void led4(sc_integer blink);
        void ledRGB(sc_integer color, sc_integer blink);

        /**
         * Nav API
         */
        void enableAvoidance(bool on);
        void setPosition(float x/*mm*/, float y/*mm*/, float h/*deg*/);
        void setSpeedAcc(sc_integer vMax /*mm/s*/, sc_integer vMaxsTurn /*°/s*/,
                sc_integer accMax /*mm/s2*/, sc_integer accMaxTurn /*°/s2*/);
        void goTo(float x /*mm*/, float y/*mm*/, eDir sens = eDir_FORWARD);
        void goToCap(float x/*mm*/, float y/*mm*/, float h/*°*/, eDir sens = eDir_FORWARD);
        void goForward(float distanceMm);
        void turnDelta(float angle/*deg*/);
        void turnTo(float angle/*deg*/);
        void faceTo(float x/*mm*/, float y/*mm*/);
        void recalFace(sc_integer border);
        void recalRear(sc_integer border);
        void stopMoving();
        bool targetReached() const;

        /**
         * Actuators
         */
        void swallow(sc_integer nbCylinders);
        void retractArms();
        void withdraw(sc_integer nbCylinders);
        void poo(sc_integer nbCylinders);

    protected:
        Robot2017& robot;
    };

} /* namespace ard */
#endif
#endif /* ROBOT_COMMON_2_AL_0_STRATFWK_STRATEGY2017_H_ */
