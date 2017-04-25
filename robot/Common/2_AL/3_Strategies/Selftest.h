/*
 * Selftest.h
 *
 *  Created on: 18 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_Selftest_Selftest_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_Selftest_Selftest_H_

#include "0_Strategy2017.h"

#ifdef BUILD_STRATEGY

#define private protected //workaround to gain instropection
#include "generated/FSM_Selftest.h"
#undef private

namespace ard
{
    /**
     * This class is used to create the instrospection that Yakindu refuses to do
     */
    class FSM_Selftest_Better: public FSM_Selftest
    {
    public:
        FSM_Selftest_Better();
        void run();
        FSM_SelftestStates getState() const {return stateConfVector[0];};
        String state2Str(FSM_SelftestStates state) const;
    protected:
        FSM_SelftestStates lastState;
    };

    /**
     *
     */
    class Selftest: public Strategy2017, public FSM_Selftest::DefaultSCI_OCB
    {
    public:
        Selftest(Robot2017* robot);

        /**---------------------------------
         * Container thread interface
         ---------------------------------*/

        //Implements IMiniThread: init the state machine
        void init() override;

        //Implements IMiniThread : method to be called by the container thread
        //                         it's expected to be called periodically
        void update(TimeMs sinceLastCall) override;

        STRAT_2017_API_ITF();

    private:
        FSM_Selftest_Better fsm;
    };

} /* namespace ard */

#endif //BUILD_STRATEGY

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_Selftest_Selftest_H_ */
