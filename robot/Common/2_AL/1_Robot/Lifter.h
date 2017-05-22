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
        void fastPoo();

        //FSM Callback : inform that poo is finished
        void pooEnded();

        //Returns true when the Lifter is ready to lift or poo the next cylinder
        bool isReady() { return fsm.getSCI_Strategy()->get_ready();};

        /**
         * FSM operation implementation
         */

        //FSM Callback
        void blocked() override;

        //FSM Callback
        void logInfo(sc_string msg) override;

        //FSM Callback
        void logError(sc_string msg) override;

    private:
        //the yakindu generated code
        FSM_Lifter fsm;

        //Actuators list
        ActuatorThread& acts;

    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_ACTUATORSCTRL_LIFTER_H_ */
