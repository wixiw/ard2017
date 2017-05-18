/*
 * Strategies.h
 *
 *  Created on: 16 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_2017_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_2017_H_

#include "Robot.h"
#include "RSP.h"
#include "LSA.h"

/**
 * Use this macro to automatically declare an FSM strategy as long as :
 * - you named the machine file FSM_[stratName].sct
 * - the machine name inisde the file is FSM_[stratName]
 * - you wish to name the created class [stratName]
 *
 * CAUTION : shall be called OUT OF ANY NAMESPACE
 *
 * This macro is too nice. Batman out! ....  Oh shit, 'forgot to drop the mic...
 */
    #define DECLARE_FSM_STRATEGY(stratName)\
    namespace ard{\
    class stratName: public Strategy2017<FSM_##stratName, FSM_##stratName::FSM_##stratName##States>{\
    public:\
        stratName(Robot2017& robot, LSAList const& lsaList) : \
        Strategy2017<FSM_##stratName, FSM_##stratName::FSM_##stratName##States>(robot, #stratName, lsaList){};\
    };}



namespace ard
{
    /**
     * This class contains a standard robot actuators/sensors interface
     * so that each build strategy doesn't have to redefine its own
     */
    template<class FSM, class States_t>
    class Strategy2017: public Action2017<FSM, States_t>
    {
    public:
        virtual ~Strategy2017() = default;
        Strategy2017(Robot2017& _robot, String const& name, LSAList const& lsaList):
                Action2017<FSM, States_t>(_robot, name),
                lsaId(eNone),
                lsaStatus(None),
                actions(lsaList)
        {
        }

        //Implements Action2017 : drive LSA
        void update(DelayMs sinceLastCall)
        {
            //Run our machine
            Action2017<FSM, States_t>::update(sinceLastCall);

            //Run the current submachine
            if( lsaStatus == InProgress)
            {
                actions.getLSA(lsaId).updateLSA(sinceLastCall);
                lsaStatus = getLSA(lsaId).getStatusLSA();
            }
        }

        //FSM API : LSA
        void startLSA(sc_integer id)
        {
            ASSERT_TEXT(lsaId == eNone, "An LSA is already in progress");
            lsaId = id;
            lsaStatus = InProgress;
            getLSA(id).startLSA();
        }

        //FSM API : LSA
        void stopLSA()
        {
            ASSERT_TEXT(lsaId != eNone, "No LSA active");
            ASSERT_TEXT(lsaStatus != None, "No LSA active");
            getLSA(lsaId).stopLSA();
            lsaId = eNone;
            lsaStatus = None;
        }

        //FSM API : LSA
        sc_integer getLSAStatus()
        {
            return (sc_integer)(lsaStatus);
        }

        //FSM API : LSA
        void goToLSAEntry(sc_integer id, sc_integer dir)
        {
            Action2017<FSM, States_t>::robot.nav.goTo(getLSA(id).getEntryPoint(), (eDir)(dir));
        }

        //FSM API : LSA
        void graphToLSAEntry(sc_integer id, sc_integer dir)
        {
            Action2017<FSM, States_t>::robot.nav.graphTo(getLSA(id).getEntryPoint(), (eDir)(dir));
        }

    protected:
        uint8_t lsaId;
        StrategyResult lsaStatus;
        LSAList const& actions;

        LSA& getLSA(uint8_t lsaId) const
        {
            return actions.getLSA(lsaId);
        }

    };
}

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_2017_H_ */


