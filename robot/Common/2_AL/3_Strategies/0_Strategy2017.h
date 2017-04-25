/*
 * Strategies.h
 *
 *  Created on: 16 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_2017_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_2017_H_

#include "RSP.h"

#ifdef BUILD_STRATEGY
#include "LSA.h"

namespace ard
{
    class Robot2017;
    
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
    class Strategy2017: public Action2017
    {
    public:
        Strategy2017(Robot2017* robot, String const& name);

        //Implements Action2017 : drive LSA
        virtual void update(DelayMs sinceLastCall);

    protected:
        uint8_t lsaId;
        LSAResult lsaStatus;
        LSA* actions[eNB_MAX_LSA];

        LSA_Dispenser lsa1;

        LSA& getLSA(eLsaId lsaId);

    };

}

#define STRAT_2017_API_ITF()\
    ACTION_2017_API_ITF()\
    void startLSA(sc_integer id);\
    sc_integer getLSAStatus(sc_integer id);\
    void goToLSAEntry(sc_integer id, sc_integer dir);

#define STRAT_2017_API_IMPL(myclass)\
ACTION_2017_API_IMPL(myclass)\
void myclass::startLSA(sc_integer id)\
{\
    lsaId = id;\
    lsaStatus = InProgress;\
    getLSA((eLsaId)(id)).start();\
}\
sc_integer myclass::getLSAStatus(sc_integer id)\
{\
    return (sc_integer)(lsaStatus);\
}\
void myclass::goToLSAEntry(sc_integer id, sc_integer dir)\
{\
    robot.nav.goToCap(getLSA((eLsaId)(id)).getEntryPoint(), (eDir)(dir));\
}

#endif //BUILD_STRATEGY

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_2017_H_ */


