/*
 * LSAList.h
 *
 *  Created on: 1 mai 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_2_LSA_LSALIST_H_
#define ROBOT_COMMON_2_AL_2_LSA_LSALIST_H_

#include "2_LSA/Dispenser.h"
#include "2_LSA/GetCylinder.h"
#include "2_LSA/Poo.h"

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
        ePoo_IA,
        eNB_MAX_LSA,
        eNone,
    } eLsaId;


    class LSAList
    {
    public:
        LSAList(Robot2017& _robot);

    //Get the LSA associated to the ID (value is checked internally by checkId())
    LSA& getLSA(uint8_t lsaId) const;

    //Check that an id is correct
    void checkId(uint8_t id) const;

    protected:
        LSA* actions[eNB_MAX_LSA];

        LSA_Dispenser       dispA;
        LSA_Dispenser       dispG;
        LSA_Dispenser       dispOppG;
        LSA_PooBorderOpp    poo1;
        LSA_PooMiddle       poo2;
        LSA_PooMiddle       poo3;
        LSA_PooMiddle       poo4;
        LSA_PooBorderOwn    poo5;
        LSA_PooStartArea    poo6;
        LSA_GetCylinder_B   takeB;
        LSA_GetCylinder_C   takeC;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_2_LSA_LSALIST_H_ */
