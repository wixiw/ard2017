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
    class LSA_DispenserMonocolor: public LSA
    {
    public:
        LSA_DispenserMonocolor(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

    class LSA_DispenserBicolor: public LSA
    {
    public:
        LSA_DispenserBicolor(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

    class LSA_DispenserOpp: public LSA
    {
    public:
        LSA_DispenserOpp(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_LSA_DISPENSER_H_ */
