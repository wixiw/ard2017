/*
 * Arms.h
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_ACTUATORSCTRL_ARMS_H_
#define ROBOT_COMMON_2_AL_ACTUATORSCTRL_ARMS_H_

#include "Lifter.h"
#include "RSP.h"
#include "generated/FSM_Arms.h"

namespace ard
{
    class ActuatorThread;

    class Arms: public PolledObject, public FSM_Arms::DefaultSCI_OCB
    {
    public:
        /**
         * @param parent : The parent thread is expected to hold all servo references
         * @param timer : a timer is required to feed yakindu timed machines
         */
        Arms(ActuatorThread& parent, TimerInterface& timer, HmiThread& hmi);

        /**---------------------------------
         * Strat API
         ---------------------------------*/
        //FSM Callback
        void start();

        //FSM Callback
        void stop();

        //Open arm and make wheel turn to swallow several cylinder on table
        void swallowNormal();

        //Open arms full to swallow a cylinder from the dispenser
        void swallowDispenser();

        //Stop turning wheels and retract arms to be able to move safely on table
        void retract();

        //Poo one cylinder with no orientation
        void fastPoo();
	
		//Returns true when the Lifter is ready to lift or poo the next cylinder
		bool isReady() { return fsm.getSCI_Strategy()->get_ready();};

        //Returns true if the Lifter is in timeout, a call to start will recover from error
        bool isInTimeout() { return fsm.getSCI_Strategy()->get_timeout();};

        //Returns true if the Lifter is in error (can not do anything else until stop, then start is called)
        bool isBlocked() { return fsm.getSCI_Strategy()->get_blocked();};
				
        //inform if a cylinder has been swallowed
        bool isCylinderTaken() { return fsm.getSCI_Strategy()->get_cylinderTaken();};

        //Reset the cylinderTaken flag
        void ackCylinderTaken(){fsm.getSCI_Strategy()->set_cylinderTaken(false);};

        /**
         * FSM operation implementation (internal API)
         */
        //FSM Callback
        void lift() override;

        //FSM Callback
        void prepareNextToPoo() override;

        //FSM Callback
		void lifterStart() override;
		void lifterStop() override;

        //FSM Callback
        bool lifterBlocked() override { return lifter->isBlocked();};

        //FSM Callback
        bool lifterTimeout() override { return lifter->isInTimeout();};

        //FSM Callback
        bool lifterReady() override { return lifter->isReady();};

        //FSM Callback
        void logInfo(sc_string msg) override;

        //FSM Callback
        void logError(sc_string msg) override;

        //FSM Callback
        void publishTimeoutOnHMI() override;

        //FSM Callback
		void publishBlockedOnHMI() override;

        //FSM Callback
        void armsCmd(sc_integer arms, sc_integer wheels);

        //FSM Callback
        void rotatorCmd(sc_integer servo, bool turn);

        //FSM Callback
        sc_integer ARM_FULL_IN() override {return ARM_MIN;};

        //FSM Callback
         sc_integer ARM_IN() override {return 450;};

        //FSM Callback
        sc_integer ARM_MID() override {return 575;};

        //FSM Callback
		sc_integer ARM_OUT() override {return ARM_MAX;};

        //FSM Callback
        sc_integer WHEEL_IDLE() override {return 500;};

        //FSM Callback
        sc_integer WHEEL_SWALLOW() override {return 0;};

        //FSM Callback
        sc_integer WHEEL_POO() override {return 1000;};

        //FSM Callback
		sc_integer ROT_RETRACTED() override {return 500;}; //TODO

		//FSM Callback
		sc_integer ROT_EXTENDED() override {return 500;}; //TODO

        /**---------------------------------
         * Container thread interface
         ---------------------------------*/

        //Implements IMiniThread: init the state machine
        void init() override;

        //Implements IMiniThread : method to be called by the container thread
        //                         it's expected to be called periodically
        void update(TimeMs sinceLastCall) override;

        void setLifter(Lifter& _lifter) {lifter = &_lifter;};

        //configure if component is simulated or not
        void setSimulation(bool simulated);

    private:
        //the yakindu generated code
        FSM_Arms fsm;

        //Actuators list
        ActuatorThread& acts;

        //Reference to lifter machine
        Lifter* lifter;

        HmiThread& hmi;

        void disableAll();
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_ACTUATORSCTRL_ARMS_H_ */
