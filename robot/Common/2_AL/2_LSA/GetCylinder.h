/*
 * GetCylinder.h
 *
 *  Created on: 16 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_GETCYLINDER_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_GETCYLINDER_H_

#include "LSA2017.h"

#define private public //workaround to gain introspection
#include "generated/FSM_LSA_GetCylinder_B.h"
#include "generated/FSM_LSA_GetCylinder_C.h"
#undef private

namespace ard
{
    typedef enum
    {
        DO_NOT_RECAL,
        RECAL
    } eLSA_GetCylinder_Recal;

    class LSA_GetCylinder_B: public LSA2017<FSM_LSA_GetCylinder_B, FSM_LSA_GetCylinder_B::FSM_LSA_GetCylinder_BStates>
    {
    public:
        LSA_GetCylinder_B(Robot2017& robot, eLSA_GetCylinder_Recal type);

        //Implements LSA
        StrategyResult getStatus() override;

        //Implements LSA
        DelayMs getNominalDuration() override;

        //Implements FSM_LSA_GetCylinder_B::DefaultSCI_OCB
        void goToEntryPoint() override;
    };

    class LSA_GetCylinder_C: public LSA2017<FSM_LSA_GetCylinder_C, FSM_LSA_GetCylinder_C::FSM_LSA_GetCylinder_CStates>
    {
    public:
        LSA_GetCylinder_C(Robot2017& robot, eLSA_GetCylinder_Recal type);

        //Implements LSA
        StrategyResult getStatus() override;

        //Implements LSA
        DelayMs getNominalDuration() override;

        //Implements FSM_LSA_GetCylinder_C::DefaultSCI_OCB
        void goToEntryPoint() override;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_GETCYLINDER_H_ */
