/*
 * Lifter.h
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_ACTUATORSCTRL_LIFTER_H_
#define ROBOT_COMMON_2_AL_ACTUATORSCTRL_LIFTER_H_

#include "RSP.h"
#ifdef BUILD_STRATEGY

#include "StratFwk.h"
#include "generated/FSM_Lifter.h"


namespace ard
{
    class ActuatorThread;

    /**
     * @param parent : The parent thread is expected to hold all servo references
     * @param timer : a timer is required to feed yakindu timed machines
     */
    class Lifter: public PolledObject
    {
    public:
        Lifter(ActuatorThread& parent, TimerInterface& timer);

        /**---------------------------------
         * Container thread interface
         ---------------------------------*/

        //Implements IMiniThread: init the state machine
        void init() override;

        //Implements IMiniThread : method to be called by the container thread
        //                         it's expected to be called periodically
        void update(TimeMs sinceLastCall) override;

        //Call this to enable the state machine (ex : after start is inside robot)
        void start();

        //Returns true when the Lifter is ready to lift or poo the next cylinder
        bool isReadyForNextCylinder() { return fsm.getSCI_Strategy()->get_readyForNext();};

        //Returns the number of sotcked cylinders
        uint8_t getCylinders() { return fsm.getSCI_Strategy()->get_stockCount();};

    private:
        //the yakindu generated code
        FSM_Lifter fsm;

        //Actuators list
        ActuatorThread& acts;
    };

} /* namespace ard */

#endif
#endif /* ROBOT_COMMON_2_AL_ACTUATORSCTRL_LIFTER_H_ */
