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
        Arms(ActuatorThread& parent, TimerInterface& timer);

        /**---------------------------------
         * Strat API
         ---------------------------------*/
        //FSM Callback
        void start();

        //FSM Callback
        void stop();

        //Open arm and make wheel turn to swallow several cylinder on table
        void swallow(uint8_t nbCylinders);

        //Stop turning wheels and retract arms to be able to move safely on table
        void retractArms();

        //Poo a certain number of cylinder (it doesn't matter if it's on table or in a container)
        void fastPoo(uint8_t nbCylinders);

        /**---------------------------------
         * Container thread interface
         ---------------------------------*/

        //FSM Callback
        void blocked() override;

        //FSM Callback
        void lift() override;

        void setLifter(Lifter& _lifter){lifter = &_lifter;};

        //Implements IMiniThread: init the state machine
        void init() override;

        //Implements IMiniThread : method to be called by the container thread
        //                         it's expected to be called periodically
        void update(TimeMs sinceLastCall) override;

        //Returns true when the Lifter is ready to lift or poo the next cylinder
        bool isReady() { return fsm.getSCI_Strategy()->get_ready();};

    private:
        //the yakindu generated code
        FSM_Arms fsm;

        //Actuators list
        ActuatorThread& acts;

        //Reference to lifter machine
        Lifter* lifter;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_ACTUATORSCTRL_ARMS_H_ */
