/*
 * LSA_Dispenser.h
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_LSA_DISPENSER_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_LSA_DISPENSER_H_

#include "StratInterfaces.h"

namespace ard
{
    class LSA_Dispenser_Monocolor: public LSA
    {
    public:
        LSA_Dispenser_Monocolor(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

    class LSA_Dispenser_Bicolor: public LSA
    {
    public:
        LSA_Dispenser_Bicolor(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

    class LSA_Dispenser_Opp: public LSA
    {
    public:
        LSA_Dispenser_Opp(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_LSA_DISPENSER_H_ */
