/*
 * Dispenser.h
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_DISPENSER_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_DISPENSER_H_

#include "StratFwk.h"

namespace ard
{
    class DispenserMonocolor: public LSA
    {
    public:
        DispenserMonocolor(Robot2017& robot);

        //Implements LSA
        virtual result execute(uint8_t nbItems = 0) override;
    };

    class DispenserBicolor: public LSA
    {
    public:
        DispenserBicolor(Robot2017& robot);

        //Implements LSA
        virtual result execute(uint8_t nbItems = 0) override;
    };

    class DispenserOpp: public LSA
    {
    public:
        DispenserOpp(Robot2017& robot);

        //Implements LSA
        virtual result execute(uint8_t nbItems = 0) override;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_DISPENSER_H_ */
