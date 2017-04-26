/*
 * Dispenser.h
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_DISPENSER_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_DISPENSER_H_

#include "0_LSAitf.h"
#ifdef BUILD_STRATEGY

#define private protected //workaround to gain instropection
#include "generated/FSM_LSA_Dispenser.h"
#undef private

namespace ard
{
    /**
     * This class is used to create the instrospection that Yakindu refuses to do
     */
    class FSM_LSA_Dispenser_Better: public FSM_LSA_Dispenser
    {
    public:
        FSM_LSA_Dispenser_Better();
        void run();
        FSM_LSA_DispenserStates getState() const {return stateConfVector[0];};
        String state2Str(FSM_LSA_DispenserStates state) const;
    protected:
        FSM_LSA_DispenserStates lastState;
    };


    typedef enum
    {
        Monocolor,
        Bicolor,
        OppBicolor
    } eLSA_DispType;

    class LSA_Dispenser: public LSA, public FSM_LSA_Dispenser::DefaultSCI_OCB
    {
    public:
        LSA_Dispenser(Robot2017* robot, eLSA_DispType type);

        //Implements IMiniThread: init the state machine
        void init() override;

        //Implements LSA
        void update(DelayMs sinceLastCall) override;

        //Implements LSA
        void start() override;

        //Implements LSA
        LSAResult isFinished() override;

        //Implements FSM_LSA_Dispenser::DefaultSCI_OCB
        void goToEntryPoint() override;

        ACTION_2017_API_ITF();

    private:
        FSM_LSA_Dispenser_Better fsm;
    };

} /* namespace ard */

#endif
#endif /* ROBOT_COMMON_2_AL_STRATEGIES_DISPENSER_H_ */
