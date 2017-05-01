/*
 * Strategies.h
 *
 *  Created on: 16 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_2017_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_2017_H_

#include "../Robot.h"
#include "RSP.h"
#include "LSA.h"

namespace ard
{
    //
    // CAUTION : keep in sync with each FSM API
    //
    typedef enum
    {
        eDisp_A,
        eDisp_G,
        eDisp_OppG,
        ePoo_1,
        ePoo_2,
        ePoo_3,
        ePoo_4,
        ePoo_5,
        eTake_B,
        eTake_C,
        eTake_D,
        eTake_E,
        eTake_F,
        eTake_OppD,
        eTake_OppE,
        eTake_OppF,
        ePoo_6,
        eNB_MAX_LSA
    } eLsaId;

    /**
     * This class contains a standard robot actuators/sensors interface
     * so that each build strategy doesn't have to redefine its own
     */
    template<class FSM, class States_t>
    class Strategy2017: public Action2017<FSM, States_t>
    {
    public:
        virtual ~Strategy2017() = default;
        Strategy2017(Robot2017& _robot, String const& name):
                Action2017<FSM, States_t>(_robot, name),
                lsaId(0),
                lsaStatus(NoLsa),
                lsa1(_robot, Monocolor)
        {
            INIT_TABLE_TO_ZERO(actions);
            actions[0] = &lsa1;
        }

        //Implements Action2017 : drive LSA
        void update(DelayMs sinceLastCall)
        {
            //Run our machine
            Action2017<FSM, States_t>::update(sinceLastCall);

            //Run the current submachine
            ASSERT(actions[lsaId]);
            if( lsaStatus == InProgress)
                actions[lsaId]->updateLSA(sinceLastCall);
            lsaStatus = actions[lsaId]->isFinished();
        }

        //FSM API : LSA
        void startLSA(sc_integer id)
        {
            lsaId = id;
            lsaStatus = InProgress;
            getLSA((eLsaId)(id)).startLSA();
        }

        sc_integer getLSAStatus(sc_integer id)
        {
            return (sc_integer)(lsaStatus);
        }

        void goToLSAEntry(sc_integer id, sc_integer dir)
        {
            Action2017<FSM, States_t>::robot.nav.goToCap(getLSA((eLsaId)(id)).getEntryPoint(), (eDir)(dir));
        }

    protected:
        uint8_t lsaId;
        LSAResult lsaStatus;
        LSA* actions[eNB_MAX_LSA];

        LSA_Dispenser lsa1;

        LSA& getLSA(eLsaId lsaId)
        {
            ASSERT(actions[lsaId]);
            return *actions[lsaId];
        }

    };

}

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_2017_H_ */


