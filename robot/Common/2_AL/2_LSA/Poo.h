/*
 * LSAPoo.h
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_POO_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_POO_H_

#include "LSA2017.h"

#define private public //workaround to gain introspection
#include "generated/FSM_LSA_PooMiddle.h"
#include "generated/FSM_LSA_PooBorderOwn.h"
#include "generated/FSM_LSA_PooBorderOpp.h"
#include "generated/FSM_LSA_PooStartArea.h"
#undef private

namespace ard
{
    typedef enum
    {
        Opp,
        Center,
        Own
    } eLSA_PooMidArea;

    class LSA_PooMiddle: public LSA2017<FSM_LSA_PooMiddle, FSM_LSA_PooMiddle::FSM_LSA_PooMiddleStates>
    {
    public:
        LSA_PooMiddle(Robot2017& robot, eLSA_PooMidArea type);

        //Implements LSA
        LSAResult isFinished() override;

        //Implements FSM_LSA_PooMiddle::DefaultSCI_OCB
        void goToEntryPoint() override;

        //Implements Action2017
        String state2Str(FSM_LSA_PooMiddle::FSM_LSA_PooMiddleStates state) const;
    };

    class LSA_PooBorderOwn: public LSA2017<FSM_LSA_PooBorderOwn, FSM_LSA_PooBorderOwn::FSM_LSA_PooBorderOwnStates>
    {
    public:
        LSA_PooBorderOwn(Robot2017& robot);

        //Implements LSA
        LSAResult isFinished() override;

        //Implements FSM_LSA_PooBorderOwn::DefaultSCI_OCB
        void goToEntryPoint() override;

        //Implements Action2017
        String state2Str(FSM_LSA_PooBorderOwn::FSM_LSA_PooBorderOwnStates state) const;
    };

    class LSA_PooBorderOpp: public LSA2017<FSM_LSA_PooBorderOpp, FSM_LSA_PooBorderOpp::FSM_LSA_PooBorderOppStates>
    {
    public:
        LSA_PooBorderOpp(Robot2017& robot);

        //Implements LSA
        LSAResult isFinished() override;

        //Implements FSM_LSA_PooBorderOpp::DefaultSCI_OCB
        void goToEntryPoint() override;

        //Implements Action2017
        String state2Str(FSM_LSA_PooBorderOpp::FSM_LSA_PooBorderOppStates state) const;
    };

    class LSA_PooStartArea: public LSA2017<FSM_LSA_PooStartArea, FSM_LSA_PooStartArea::FSM_LSA_PooStartAreaStates>
    {
    public:
        LSA_PooStartArea(Robot2017& robot);

        //Implements LSA
        LSAResult isFinished() override;

        //Implements FSM_LSA_PooStartArea::DefaultSCI_OCB
        void goToEntryPoint() override;

        //Implements Action2017
        String state2Str(FSM_LSA_PooStartArea::FSM_LSA_PooStartAreaStates state) const;
    };

} /* namespace ard */


//FSM_LSA_PooStartArea
//    class PooBorder: public LSA
//    {
//    public:
//        PooBorder(Robot2017& robot);
//
//        //Implements LSA
//        virtual LSAResult execute(uint8_t nbItems = 0) override;
//    };
//
//    class PooBorderOpp: public LSA
//    {
//    public:
//        PooBorderOpp(Robot2017& robot);
//
//        //Implements LSA
//        virtual LSAResult execute(uint8_t nbItems = 0) override;
//    };
//
//} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_POO_H_ */
