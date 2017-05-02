/*
 * Dispenser.h
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_DISPENSER_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_DISPENSER_H_

#include "LSA2017.h"

#define private public //workaround to gain introspection
#include "generated/FSM_LSA_Dispenser.h"
#undef private

namespace ard
{
    typedef enum
    {
        Monocolor,
        Bicolor,
        OppBicolor
    } eLSA_DispType;

    class LSA_Dispenser: public LSA2017<FSM_LSA_Dispenser, FSM_LSA_Dispenser::FSM_LSA_DispenserStates>
    {
    public:
        LSA_Dispenser(Robot2017& robot, eLSA_DispType type);

        //Implements LSA
        LSAResult isFinished() override;

        //Implements FSM_LSA_Dispenser::DefaultSCI_OCB
        void goToEntryPoint() override;

        //FSM API : Use this hide in the FSM which dispenser is used
        void informWithdraw_generic() override;

        //FSM API : Use this hide in the FSM which dispenser is used
        sc_integer dispenserCount() override;

    protected:
        eLSA_DispType type;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_DISPENSER_H_ */
