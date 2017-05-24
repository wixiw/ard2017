/*
 * Lifter.h
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_ACTUATORSCTRL_LIFTER_H_
#define ROBOT_COMMON_2_AL_ACTUATORSCTRL_LIFTER_H_

#include "RSP.h"
#include "generated/FSM_Lifter.h"


namespace ard
{
    class ActuatorThread;

    /**
     * @param parent : The parent thread is expected to hold all servo references
     * @param timer : a timer is required to feed yakindu timed machines
     */
    class Lifter: public PolledObject, public FSM_Lifter::DefaultSCI_OCB
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

        //configure if component is simulated or not
        void setSimulation(bool simulated);

        /**
         * Strategy API
         */

        //FSM Callback
        void start();

        //FSM Callback
        void stop();

        //FSM Callback
        void lift();

        //FSM Callback
        void prepareNextToPoo();

        //Returns true when the Lifter is ready to lift or poo the next cylinder
        bool isReady() { return fsm.getSCI_Strategy()->get_ready();};

        //Returns true if the Lifter is in timeout, a call to start will recover from error
        bool isInTimeout() { return fsm.getSCI_Strategy()->get_timeout();};

        //Returns true if the Lifter is in error (can not do anything else until stop, then start is called)
        bool isBlocked() { return fsm.getSCI_Strategy()->get_blocked();};

        /**
         * FSM operation implementation
         */

        //FSM Callback
        void logInfo(sc_string msg) override;

        //FSM Callback
        void logError(sc_string msg) override;

        //FSM Callback
        sc_integer UP_CMD() override {return LIFTER_MIN;};

        //FSM Callback
        sc_integer DOWN_CMD() override {return 655;};

        //FSM Callback
        sc_integer FULL_DOWN_CMD() override {return LIFTER_MAX;};

    private:
        //the yakindu generated code
        FSM_Lifter fsm;

        //Actuators list
        ActuatorThread& acts;

    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_ACTUATORSCTRL_LIFTER_H_ */
