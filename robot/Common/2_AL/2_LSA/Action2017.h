/*
 * Action2017.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_
#define ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_

#include "../Robot.h"
#include "../Robot.h"
#include "RSP.h"

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
            fsm.setTimer(&robot.getFsmTimer());
            fsm.setDefaultSCI_OCB(this);
        }

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
        }

        /**
         * HMI
         */
        void beep(sc_integer nb)
        {
            robot.hmi.buzzer.bip((uint8_t)(nb));
        }

        void led1(sc_integer blink)
        {
            ASSERT(blink < eLedState_Max);
            robot.hmi.led1.set((eLedState)(blink));
        }

        void led2(sc_integer blink)
        {
            ASSERT(blink < eLedState_Max);
            robot.hmi.led2.set((eLedState)(blink));
        }

        void led3(sc_integer blink)
        {
            ASSERT(blink < eLedState_Max);
            robot.hmi.led3.set((eLedState)(blink));
        }

        void led4(sc_integer blink)
        {
            ASSERT(blink < eLedState_Max);
            robot.hmi.led4.set((eLedState)(blink));
        }

        void ledRGB(sc_integer color, sc_integer blink)
        {
            ASSERT(color < eRgb_Max);
            ASSERT(blink < eLedState_Max);
            robot.hmi.ledRGB.set((eRgb)(color), eLedState(blink));
        }

        bool isStartPlugged()
        {
            return robot.hmi.isStartPlugged();
        }

        bool isColorSwitchOnPrefered()
        {
            return robot.hmi.isColorSwitchOnPrefered();
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
            LOG_DEBUG(msg);
        }

        void logInfo(sc_string msg)
        {
            LOG_INFO(msg);
        }

        void logError(sc_string msg)
        {
            LOG_ERROR(msg);
        }

        /**
         * General
         */
        void dieMotherFucker()
        {
            robot.dieMotherFucker();
        }

        sc_integer xav()
        {
            return robot.getConfig().xav;
        }
        sc_integer xar()
        {
            return robot.getConfig().xar;
        }
        sc_integer yside()
        {
            return robot.getConfig().yside;
        }
        sc_integer xavExtended()
        {
            return robot.getConfig().xavExtended;
        }
        sc_integer xouter()
        {
            return robot.getConfig().xouter;
        }
        sc_integer getRemainingTime()
        {
            return robot.chrono.getStrategyRemainingTime();
        }

        /**
         * Navigation
         */
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
            ASSERT(sens == -1 || sens == 1);
            robot.nav.goTo(x, y, (eDir)(sens));
        }

        void goToCap(sc_real x, sc_real y, sc_real h, sc_integer sens)
        {
            ASSERT(sens == -1 || sens == 1);
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
            ASSERT(border < eTableBorder_MAX);
            robot.nav.recalFace((eTableBorder)(border));
        }

        void recalRear(sc_integer border)
        {
            ASSERT(border < eTableBorder_MAX);
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

        sc_boolean omronFrontLeft()
        {
            return robot.nav.omronFrontLeft.read();
        }

        sc_boolean omronFrontRight()
        {
            return robot.nav.omronFrontRight.read();
        }

        sc_boolean omronRearLeft()
        {
            return robot.nav.omronRearLeft.read();
        }

        sc_boolean omronRearRight()
        {
            return robot.nav.omronRearRight.read();
        }

        sc_boolean switchRecalFL()
        {
            return robot.nav.switchRecalFL.read();
        }

        sc_boolean switchRecalFR()
        {
            return robot.nav.switchRecalFR.read();
        }

        sc_boolean switchRecalRC()
        {
            return robot.nav.switchRecalRC.read();
        }

        /**
         * Actionneurs
         */
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

        sc_boolean lifterAtTarget()
        {
            return robot.actuators.servoLifter.isTargetReached();
        }

        void arms(sc_integer left, sc_integer right)
        {
            ASSERT(left <= 1000);
            ASSERT(right <= 1000);
            robot.actuators.servoLeftArm.goTo((uint16_t)left);
            robot.actuators.servoRightArm.goTo((uint16_t)right);
        }

        sc_boolean armsAtTarget()
        {
            return robot.actuators.servoLeftArm.isTargetReached()
                    &&robot.actuators.servoRightArm.isTargetReached();
        }

        void turnCylinder(sc_boolean on)
        {
            return robot.actuators.turnCylinder(on);
        }

        void faceUpCylinder()
        {
            robot.actuators.faceUpCylinder();
        }

        sc_integer getFaceUpStatus()
        {
            return robot.actuators.getFaceUpStatus();
        }

        sc_boolean switchArmLout()
        {
            return robot.actuators.switchArmLout.read();
        }

        sc_boolean switchArmLin()
        {
            return robot.actuators.switchArmLin.read();
        }

        sc_boolean switchArmRout()
        {
            return robot.actuators.switchArmRout.read();
        }

        sc_boolean switchArmRin()
        {
            return robot.actuators.switchArmRin.read();
        }

        sc_boolean omronCylinder()
        {
            return robot.actuators.omronCylinder.read();
        }

        sc_boolean switchCylinder()
        {
            return robot.actuators.switchCylinder.read();
        }

        sc_boolean switchLifterUp()
        {
            return robot.actuators.switchLifterUp.read();
        }

        sc_boolean switchLifterDown()
        {
            return robot.actuators.switchLifterDown.read();
        }

        sc_integer stockColor()
        {
            return robot.actuators.stockColor.getColor();
        }

        /**
         * Strategy Info
         */
        sc_integer score()
        {
            return robot.stratInfo.data.score;
        }

        sc_integer robotStockCount()
        {
            return robot.stratInfo.data.robotCylinderStockNb;
        }

        sc_integer nextCylinderColor()
        {
            NOT_IMPLEMENTED();
            return 0;
            //TODO WIX return robot.stratInfo.data.stock[robot.stratInfo.data.robotCylinderStockNb];
        }

        sc_integer containerCount(sc_integer containerId)
        {
            switch (containerId) {
                case 1:
                    return robot.stratInfo.data.containerBorderOppNb;
                    break;
                case 2:
                    return robot.stratInfo.data.containerMidleOppNb;
                    break;
                case 3:
                    return robot.stratInfo.data.containerMidleCenterNb;
                    break;
                case 4:
                    return robot.stratInfo.data.containerMidleOwnNb;
                    break;
                case 5:
                    return robot.stratInfo.data.containerBorderNb;
                    break;
                case 6:
                    return robot.stratInfo.data.containerStartNb;
                    break;
                default:
                    ASSERT(false);
                    return 0;
                    break;
            }
        }

        sc_integer dispenserA_Count()
        {
            return robot.stratInfo.data.dispenserMonocolorNb;
        }

        sc_integer dispenserG_Count()
        {
            return robot.stratInfo.data.dispenserBicolorNb;
        }

        sc_integer dispenserOppG_Count()
        {
            return robot.stratInfo.data.dispenserOppNb;
        }

        void informWithdraw_A(sc_integer nb)
        {
            robot.stratInfo.informWithdraw_A((uint8_t)nb);
        }

        void informWithdraw_G(sc_integer nb)
        {
            robot.stratInfo.informWithdraw_G((uint8_t)nb);
        }

        void informWithdraw_OppG(sc_integer nb)
        {
            robot.stratInfo.informWithdraw_OppG((uint8_t)nb);
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

        void informPooed_1(sc_integer nb)
        {
            robot.stratInfo.informPooed_1((uint8_t)nb);
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
    };


} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_ */
