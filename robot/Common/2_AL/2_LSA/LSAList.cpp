/*
 * LSAList.cpp
 *
 *  Created on: 1 mai 2017
 *      Author: wix
 */

#include "LSAList.h"

using namespace ard;

LSAList::LSAList(Robot2017& _robot):
    dispA   (_robot, Monocolor),
    dispG   (_robot, Bicolor),
    dispOppG(_robot, OppBicolor),
    poo1(_robot),
    poo2(_robot, Opp),
    poo3(_robot, Center),
    poo4(_robot, Own),
    poo5(_robot),
    poo6(_robot),
    takeB(_robot, RECAL),
    takeC(_robot, RECAL)
{
    INIT_TABLE_TO_ZERO(actions);
    actions[eDisp_A]        = &dispA;
    actions[eDisp_G]        = &dispG;
    actions[eDisp_OppG]     = &dispOppG;
    actions[ePoo_1]         = &poo1;
    actions[ePoo_2]         = &poo2;
    actions[ePoo_3]         = &poo3;
    actions[ePoo_4]         = &poo4;
    actions[ePoo_5]         = &poo5;
    actions[eTake_B]        = &takeB;
    actions[eTake_C]        = &takeC;
    actions[ePoo_6]         = &poo6;
}

LSA& LSAList::getLSA(uint8_t lsaId) const
{
    checkId(lsaId);
    ASSERT(actions[lsaId]);
    return *actions[lsaId];
}

void LSAList::checkId(uint8_t id) const
{
    ASSERT(id < eNB_MAX_LSA);
    //Check for unimplemented LSA
    ASSERT_TEXT(id != eTake_D, "LSA_D is not implemented");
    ASSERT_TEXT(id != eTake_E, "LSA_E is not implemented");
    ASSERT_TEXT(id != eTake_F, "LSA_F is not implemented");
    ASSERT_TEXT(id != eTake_OppD, "LSA_OppD is not implemented");
    ASSERT_TEXT(id != eTake_OppE, "LSA_OppE is not implemented");
    ASSERT_TEXT(id != eTake_OppF, "LSA_OppF is not implemented");
    ASSERT_TEXT(id != eIA, "LSA_IA is not implemented");
}
