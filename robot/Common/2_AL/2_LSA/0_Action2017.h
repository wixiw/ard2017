/*
 * Action2017.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_
#define ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_

#include "../Robot2017.h"
#include "RSP.h"
#include "Robot2017.h"

namespace ard
{
    template<class FSM, class States_t>
    class Action2017: public IStrategy, public FSM::DefaultSCI_OCB
    {
    public:
        Action2017(Robot2017& _robot, String const& name)
                : IStrategy(name),
                  robot(_robot)
        {
            ASSERT_TEXT(fsm.maxOrthogonalStates == 1, "Orthogonoal regions limited to 1 on ARD.");
            fsm.setTimer(&robot.lifecycle.fsmTimer);
            fsm.setDefaultSCI_OCB(this);
        }

        //Convert a state to string for logging
        virtual String state2Str(States_t state) const = 0;

        //Implements IStrategy: init the state machine
        void init() override
        {
            PolledObject::init();
            fsm.init();
        }

        virtual void start()
        {
            fsm.init();
            fsm.enter();
        }

        //Implements LSA
        void update(DelayMs sinceLastCall) override
        {
            fsm.runCycle();
            States_t newState = getState();
            if( lastState != newState)
            {
                LOG_INFO(String("[LSA_Dispenser] state changed :  ") + state2Str(lastState) + " => state " + state2Str(newState));
                lastState = newState;
            }
        }

        //Provide a state accessor as Yakindu doesn't provide it.
        States_t getState() const {return fsm.stateConfVector[0];};

        void beep(sc_integer nb)
        {
            robot.buzzer().bip((uint8_t)(nb));
        }

        void led1(sc_integer blink)
        {
            robot.hmi.led1.set((eLedState)(blink));
        }

        void led2(sc_integer blink)
        {
            robot.hmi.led2.set((eLedState)(blink));
        }

        void led3(sc_integer blink)
        {
            robot.hmi.led3.set((eLedState)(blink));
        }

        void led4(sc_integer blink)
        {
            robot.hmi.led4.set((eLedState)(blink));
        }

        void ledRGB(sc_integer color, sc_integer blink)
        {
            robot.hmi.ledRGB.set((eRgb)(color), eLedState(blink));
        }

        bool isStartPlugged()
        {
            return robot.isStartPlugged();
        }

        bool isColorSwitchOnPrefered()
        {
            return robot.isColorSwitchOnPrefered();
        }

        bool isUser1SwitchOn()
        {
            return robot.hmi.user1.read();
        }

        bool isUser2SwitchOn()
        {
            return robot.hmi.user2.read();
        }

        void logDebug(sc_string msg)
        {
            LOG_DEBUG(String(msg));
        }

        void logInfo(sc_string msg)
        {
            LOG_INFO(String(msg));
        }

        void logError(sc_string msg)
        {
            LOG_ERROR(String(msg));
        }

        void enableAvoidance(sc_boolean on)
        {
            robot.nav.enableAvoidance(on);
        }

        void setPosition(sc_real x, sc_real y, sc_real h)
        {
            robot.nav.setPosition(x, y, h);
        }

        void setSpeedAcc(sc_integer vMax, sc_integer vMaxsTurn, sc_integer accMax, sc_integer accMaxTurn)
        {
            robot.nav.setSpeedAcc((uint16_t)vMax, (uint16_t)vMaxsTurn, (uint16_t)accMax, (uint16_t)accMaxTurn);
        }

        void goTo_ID(sc_real x, sc_real y, sc_integer sens)
        {
            robot.nav.goTo(x, y, (eDir)(sens));
        }

        void goToCap(sc_real x, sc_real y, sc_real h, sc_integer sens)
        {
            robot.nav.goToCap(x, y, h, (eDir)(sens));
        }

        void goForward(sc_real distanceMm)
        {
            robot.nav.goForward(distanceMm);
        }

        void turnDelta(sc_real angle)
        {
            robot.nav.turnDelta(angle);
        }

        void turnTo(sc_real angle)
        {
            robot.nav.turnTo(angle);
        }

        void faceTo(sc_real x, sc_real y)
        {
            robot.nav.faceTo(x, y);
        }

        void recalFace(sc_integer border)
        {
            robot.nav.recalFace((eTableBorder)(border));
        }

        void recalRear(sc_integer border)
        {
            robot.nav.recalRear((eTableBorder)(border));
        }

        void stopMoving()
        {
            robot.nav.stopMoving();
        }

        sc_boolean targetReached()
        {
            return robot.nav.targetReached();
        }

        void swallow(sc_boolean on)
        {
            robot.actuators.swallow(on);
        }

        void turnWheels(sc_integer on)
        {
            robot.actuators.turnWheels((uint8_t)on);
        }

        void lifter(sc_boolean up)
        {
            robot.actuators.lifterCmd(up);
        }

        void arms(sc_integer left, sc_integer right)
        {
            robot.actuators.servoLeftArm.write((uint16_t)left);
            robot.actuators.servoRightArm.write((uint16_t)right);
        }

        void informWithdraw_A(sc_integer nb)
        {
            robot.stratInfo.informWithdraw_A((uint8_t)nb);
        }

        void informWithdraw_G(sc_integer nb)
        {
            robot.stratInfo.informWithdraw_G((uint8_t)nb);
        }

        void informWithdraw_Opp_G(sc_integer nb)
        {
            robot.stratInfo.informWithdraw_Opp_G((uint8_t)nb);
        }

        void informPooed_3(sc_integer nb)
        {
            robot.stratInfo.informPooed_3((uint8_t)nb);
        }

        void informPooed_4(sc_integer nb)
        {
            robot.stratInfo.informPooed_4((uint8_t)nb);
        }

        void informPooed_2(sc_integer nb)
        {
            robot.stratInfo.informPooed_2((uint8_t)nb);
        }

        void informPooed_5(sc_integer nb)
        {
            robot.stratInfo.informPooed_5((uint8_t)nb);
        }

        void informPooed_Opp_5(sc_integer nb)
        {
            robot.stratInfo.informPooed_Opp_5((uint8_t)nb);
        }

        void informPooed_6(sc_integer nb)
        {
            robot.stratInfo.informPooed_6((uint8_t)nb);
        }

        void informPooed_OnTable(sc_integer nb)
        {
            robot.stratInfo.informPooed_OnTable((uint8_t)nb);
        }

        void informTaken_D()
        {
            robot.stratInfo.informTaken_D();
        }

        void informTaken_F()
        {
            robot.stratInfo.informTaken_F();
        }

        void informTaken_E()
        {
            robot.stratInfo.informTaken_E();
        }

        void informTaken_B()
        {
            robot.stratInfo.informTaken_B();
        }

        void informTaken_C()
        {
            robot.stratInfo.informTaken_C();
        }

        void informTaken_Opp_D()
        {
            robot.stratInfo.informTaken_Opp_D();
        }

        void informTaken_Opp_F()
        {
            robot.stratInfo.informTaken_Opp_F();
        }

        void informTaken_Opp_E()
        {
            robot.stratInfo.informTaken_Opp_E();
        }

        void informPushedAway_D()
        {
            robot.stratInfo.informPushedAway_D();
        }

        void informPushedAway_F()
        {
            robot.stratInfo.informPushedAway_F();
        }

        void informPushedAway_E()
        {
            robot.stratInfo.informPushedAway_E();
        }

        void informPushedAway_B()
        {
            robot.stratInfo.informPushedAway_B();
        }

        void informPushedAway_C()
        {
            robot.stratInfo.informPushedAway_C();
        }

        void informPushedAway_Opp_D()
        {
            robot.stratInfo.informPushedAway_Opp_D();
        }

        void informPushedAway_Opp_F()
        {
            robot.stratInfo.informPushedAway_Opp_F();
        }

        void informPushedAway_Opp_E()
        {
            robot.stratInfo.informPushedAway_Opp_E();
        }

    protected:
        Robot2017& robot;
        FSM fsm;

    private:
        States_t lastState;
    };


} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_ */