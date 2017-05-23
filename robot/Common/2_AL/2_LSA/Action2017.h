/*
 * Action2017.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_
#define ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_

#include "Robot.h"
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

        virtual void start() override
        {
            fsm.init();
            fsm.enter();
            status = InProgress;
        }

        virtual void stop() override
        {
            fsm.exit();
            status = None;
        }

        //Implements LSA
        void update(DelayMs sinceLastCall) override
        {
            fsm.runCycle();
        }

        /**
         * HMI
         */
        void beep(sc_integer nb) override
        {
            robot.hmi.buzzer.bip((uint8_t)(nb));
        }

        void led1(sc_integer blink) override
        {
            ASSERT(blink < eLedState_Max);
            robot.hmi.led1.set((eLedState)(blink));
        }

        void led2(sc_integer blink) override
        {
            ASSERT(blink < eLedState_Max);
            robot.hmi.led2.set((eLedState)(blink));
        }

        void led3(sc_integer blink) override
        {
            ASSERT(blink < eLedState_Max);
            robot.hmi.led3.set((eLedState)(blink));
        }

        void led4(sc_integer blink) override
        {
            ASSERT(blink < eLedState_Max);
            robot.hmi.led4.set((eLedState)(blink));
        }

        void ledRGB(sc_integer color, sc_integer blink) override
        {
            ASSERT(color < eRgb_Max);
            ASSERT(blink < eLedState_Max);
            robot.hmi.ledRGB.set((eRgb)(color), eLedState(blink));
        }

        void logDebug(sc_string msg) override
        {
            LOG_DEBUG(String(msg));
        }

        void logInfo(sc_string msg) override
        {
            LOG_INFO(String(msg));
        }

        void logError(sc_string msg) override
        {
            LOG_ERROR(String(msg));
        }

        /**
         * General
         */
        void dieMotherFucker() override
        {
            robot.dieMotherFucker();
        }

        sc_integer xav() override
        {
            return robot.getConfig().xav;
        }
        sc_integer xar() override
        {
            return robot.getConfig().xar;
        }
        sc_integer yside() override
        {
            return robot.getConfig().yside;
        }
        sc_integer xavExtended() override
        {
            return robot.getConfig().xavExtended;
        }
        sc_integer xarExtended() override
        {
            return robot.getConfig().xarExtended;
        }
        sc_integer xouter() override
        {
            return robot.getConfig().xouter;
        }
        void setStatus(sc_integer _status) override
        {
            status = (StrategyResult)_status;
        }
        sc_boolean matchColor() override
        {
            return robot.stratInfo.getColor() == eColor_PREF;
        }
        sc_boolean robotType() override
        {
            return robot.isPen();
        }
        sc_boolean isSimu() override
        {
            return robot.lifecycle.isSimulated();
        }
        sc_integer matchDuration() override
        {
            return robot.chrono.getTime();
        }
        sc_integer matchRemainingTime() override
        {
            return robot.chrono.getStrategyRemainingTime();
        }

        /**
         * Navigation
         */
        sc_integer x(sc_boolean sym) override
		{
        	return robot.nav.getPosition(sym).x;
		}

        sc_integer y(sc_boolean sym) override
		{
        	return robot.nav.getPosition(sym).y;
		}
        sc_integer h(sc_boolean sym) override
		{
        	return robot.nav.getPosition(sym).h;
		}

        void enableAvoidance(sc_boolean on) override
        {
            robot.detection.enableAvoidance(on);
        }

        void setPosition(sc_real x, sc_real y, sc_real h) override
        {
            robot.nav.setPosition(x, y, h);
        }

        void setSpeedAcc(sc_integer vMax, sc_integer vMaxsTurn, sc_integer accMax, sc_integer accMaxTurn) override
        {
            robot.kinematics.setSpeedAcc((uint16_t)vMax, (uint16_t)vMaxsTurn, (uint16_t)accMax, (uint16_t)accMaxTurn);
        }

        void goTo_ID(sc_real x, sc_real y, sc_integer sens, sc_boolean sym) override
        {
            ASSERT(sens == -1 || sens == 1);
            robot.nav.goTo(x, y, (eDir)(sens), sym);
        }

        void goToCap(sc_real x, sc_real y, sc_real h, sc_integer sens, sc_boolean sym) override
        {
            ASSERT(sens == -1 || sens == 1);
            robot.nav.goToCap(x, y, h, (eDir)(sens), sym);
        }

        void goForward(sc_real distanceMm) override
        {
            robot.nav.goForward(distanceMm);
        }

        void turnDelta(sc_real angle, sc_boolean sym) override
        {
            robot.nav.turnDelta(angle, sym);
        }

        void turnTo(sc_real angle, sc_boolean sym) override
        {
            robot.nav.turnTo(angle, sym);
        }

        void faceTo(sc_real x, sc_real y, sc_boolean sym) override
        {
            robot.nav.faceTo(x, y, sym);
        }

        void rearTo(sc_real x, sc_real y, sc_boolean sym) override
        {
            robot.nav.rearTo(x, y, sym);
        }

        void recalFace(sc_integer border) override
        {
            Distance distance = robot.conf.xouter() - robot.conf.xav() + RECAL_ESCAPE_MARGIN;
            robot.nav.recalFace((eTableBorder)(border), distance);
        }

        void recalRear(sc_integer border) override
        {
            Distance distance = robot.conf.xouter() - robot.conf.xar() + RECAL_ESCAPE_MARGIN;
            robot.nav.recalRear((eTableBorder)(border), distance);
        }

        void stopMoving() override
        {
            robot.nav.stopMoving();
        }

        sc_boolean targetReached() override
        {
            return robot.nav.targetReached();
        }

        sc_boolean blocked() override
        {
            return robot.nav.blocked();
        }

        sc_boolean switchRecalFL() override
        {
            return robot.nav.switchRecalFL.read();
        }

        sc_boolean switchRecalFR() override
        {
            return robot.nav.switchRecalFR.read();
        }

        sc_boolean switchRecalRC() override
        {
            return robot.nav.switchRecalRC.read();
        }

        /**
         * Detection
         */
        sc_boolean omronFront() override
        {
            return robot.detection.omronFront.read();
        }

        sc_boolean omronRear() override
        {
            return robot.detection.omronRear.read();
        }
        sc_boolean isOpponentAhead() override
        {
            return robot.detection.isOpponentAhead(robot.nav.getPosition());
        }

        sc_boolean isOpponentBehind() override
        {
            return robot.detection.isOpponentBehind(robot.nav.getPosition(SYM_POS));
        }

        /**
         * Actionneurs
         */
        void swallow(sc_integer on) override
        {
        	switch ((eArmsCmd) on){
        		case AC_RETRACT :
        			robot.actuators.arms.retract();
        			break;
        		case AC_SWALLOW_NORMAL :
        			robot.actuators.arms.swallowNormal();
        			break;
        		case AC_SWALLOW_DISPENSER :
        			robot.actuators.arms.swallowDispenser();
        			break;
        		default:;
        	}
        }

        void turnWheels(sc_integer on) override
        {
            robot.actuators.turnWheels((eWheelsCmd)on);
        }

        void lifter(sc_boolean up) override
        {
            robot.actuators.lifterCmd(up);
        }

        sc_boolean lifterAtTarget() override
        {
            return robot.actuators.servoLifter.isTargetReached();
        }

        void arms(sc_integer left, sc_integer right) override
        {
            ASSERT(left <= 1000);
            ASSERT(right <= 1000);
            robot.actuators.servoLeftArm.goTo((uint16_t)left);
            robot.actuators.servoRightArm.goTo((uint16_t)right);
        }

        sc_boolean armsAtTarget() override
        {
            return robot.actuators.servoLeftArm.isTargetReached()
                    &&robot.actuators.servoRightArm.isTargetReached();
        }

        sc_boolean armsReady() override
        {
            return robot.actuators.arms.isReady();
        }

        sc_boolean lifterReady() override
        {
            return robot.actuators.lifter.isReady();
        }

        void turnCylinder(sc_boolean on) override
        {
            return robot.actuators.turnCylinder(on);
        }

        void faceUpCylinder() override
        {
            robot.actuators.faceUpCylinder();
        }

        sc_integer getFaceUpStatus() override
        {
            return robot.actuators.getFaceUpStatus();
        }

        void actCmd(sc_integer cmdId) override
        {
            return robot.actuators.actCmd((eActCmd)(cmdId));
        }

        sc_boolean switchArmLout() override
        {
            return robot.actuators.switchArmLout.read();
        }

        sc_boolean switchArmLin() override
        {
            return robot.actuators.switchArmLin.read();
        }

        sc_boolean switchArmRout() override
        {
            return robot.actuators.switchArmRout.read();
        }

        sc_boolean switchArmRin() override
        {
            return robot.actuators.switchArmRin.read();
        }

        sc_boolean omronCylinder() override
        {
            return robot.actuators.omronCylinder.read();
        }
        
        sc_boolean switchLifterUp() override
        {
            return robot.actuators.switchLifterUp.read();
        }

        sc_boolean switchLifterDown() override
        {
            return robot.actuators.switchLifterDown.read();
        }

        sc_integer stockColor() override
        {
            return robot.actuators.stockColor.getColor();
        }

        /**
         * Strategy Info
         */
        sc_integer score() override
        {
            return robot.stratInfo.data.score;
        }

        sc_integer robotStockCount() override
        {
            return robot.stratInfo.data.stock_count;
        }

        sc_integer nextCylinderColor() override
        {
            return robot.stratInfo.data.stock[robot.stratInfo.data.stock_count];
        }

        sc_integer containerCount(sc_integer containerId) override
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

        sc_integer dispenserA_Count() override
        {
            return robot.stratInfo.data.dispenserMonocolorNb;
        }

        sc_integer dispenserG_Count() override
        {
            return robot.stratInfo.data.dispenserBicolorNb;
        }

        sc_integer dispenserOppG_Count() override
        {
            return robot.stratInfo.data.dispenserOppNb;
        }

        void informWithdraw_A(sc_integer nb) override
        {
            robot.stratInfo.informWithdraw_A((uint8_t)nb);
        }

        void informWithdraw_G(sc_integer nb) override
        {
            robot.stratInfo.informWithdraw_G((uint8_t)nb);
        }

        void informWithdraw_OppG(sc_integer nb) override
        {
            robot.stratInfo.informWithdraw_OppG((uint8_t)nb);
        }

        void informPooed_3(sc_integer nb) override
        {
            robot.stratInfo.informPooed_3((uint8_t)nb);
        }

        void informPooed_4(sc_integer nb) override
        {
            robot.stratInfo.informPooed_4((uint8_t)nb);
        }

        void informPooed_2(sc_integer nb) override
        {
            robot.stratInfo.informPooed_2((uint8_t)nb);
        }

        void informPooed_5(sc_integer nb) override
        {
            robot.stratInfo.informPooed_5((uint8_t)nb);
        }

        void informPooed_1(sc_integer nb) override
        {
            robot.stratInfo.informPooed_1((uint8_t)nb);
        }

        void informPooed_6(sc_integer nb) override
        {
            robot.stratInfo.informPooed_6((uint8_t)nb);
        }

        void informPooed_OnTable(sc_integer nb) override
        {
            robot.stratInfo.informPooed_OnTable((uint8_t)nb);
        }

        void informTaken_D() override
        {
            robot.stratInfo.informTaken_D();
        }

        void informTaken_F() override
        {
            robot.stratInfo.informTaken_F();
        }

        void informTaken_E() override
        {
            robot.stratInfo.informTaken_E();
        }

        void informTaken_B() override
        {
            robot.stratInfo.informTaken_B();
        }

        void informTaken_C() override
        {
            robot.stratInfo.informTaken_C();
        }

        void informTaken_Opp_D() override
        {
            robot.stratInfo.informTaken_Opp_D();
        }

        void informTaken_Opp_F() override
        {
            robot.stratInfo.informTaken_Opp_F();
        }

        void informTaken_Opp_E() override
        {
            robot.stratInfo.informTaken_Opp_E();
        }

        void informPushedAway_D() override
        {
            robot.stratInfo.informPushedAway_D();
        }

        void informPushedAway_F() override
        {
            robot.stratInfo.informPushedAway_F();
        }

        void informPushedAway_E() override
        {
            robot.stratInfo.informPushedAway_E();
        }

        void informPushedAway_B() override
        {
            robot.stratInfo.informPushedAway_B();
        }

        void informPushedAway_C() override
        {
            robot.stratInfo.informPushedAway_C();
        }

        void informPushedAway_Opp_D() override
        {
            robot.stratInfo.informPushedAway_Opp_D();
        }

        void informPushedAway_Opp_F() override
        {
            robot.stratInfo.informPushedAway_Opp_F();
        }

        void informPushedAway_Opp_E() override
        {
            robot.stratInfo.informPushedAway_Opp_E();
        }

    protected:
        Robot2017& robot;
        FSM fsm;
    };


} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_0_STRATFWK_ACTION2017_H_ */
