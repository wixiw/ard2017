/*
 * Arms.h
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_ACTUATORSCTRL_ARMS_H_
#define ROBOT_COMMON_2_AL_ACTUATORSCTRL_ARMS_H_

#include "RSP.h"
#ifdef BUILD_STRATEGY

#include "generated/FSM_Arms.h"
#include "StratFwk.h"
#include "Lifter.h"

namespace ard
{
    class ActuatorThread;

    class Arms: public PolledObject
    {
    public:
        /**
         * @param parent : The parent thread is expected to hold all servo references
         * @param timer : a timer is required to feed yakindu timed machines
         */
        Arms(ActuatorThread& parent, TimerInterface& timer);

        /**---------------------------------
         * Strat API
         ---------------------------------*/
        //Open arm and make wheel turn to swallow several cylinder on table
        void swallow(uint8_t nbCylinders);

        //Stop turning wheels and retract arms to be able to move safely on table
        void retractArms();

        //Take a certain number of cylinders from the dispenser
        void withdraw(uint8_t nbCylinders);

        //Poo a certain number of cylinder (it doesn't matter if it's on table or in a container)
        void poo(uint8_t nbCylinders);

        /**---------------------------------
         * Container thread interface
         ---------------------------------*/

        void setLifter(Lifter& _lifter){lifter = &_lifter;};

        //Implements IMiniThread: init the state machine
        void init() override;

        //Implements IMiniThread : method to be called by the container thread
        //                         it's expected to be called periodically
        void update(TimeMs sinceLastCall) override;

        //Call this to enable the state machine (ex : after start is inside robot)
        void start();

        //Retrive the Strategy API to emit/receive events
        FSM_Arms::SCI_Strategy* getEvents();

    private:
        //the yakindu generated code
        FSM_Arms fsm;

        //Actuators list
        ActuatorThread& acts;

        //Reference to lifter machine
        Lifter* lifter;
    };

} /* namespace ard */

#endif
#endif /* ROBOT_COMMON_2_AL_ACTUATORSCTRL_ARMS_H_ */
