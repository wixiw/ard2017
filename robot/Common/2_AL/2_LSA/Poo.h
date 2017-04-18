/*
 * LSAPoo.h
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_POO_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_POO_H_

#include "StratFwk.h"

namespace ard
{

    class PooMiddleCenter: public LSA
    {
    public:
        PooMiddleCenter(Robot2017& robot);

        //Implements LSA
        virtual result execute(uint8_t nbItems = 0) override;
    };

    class PooMiddleOwn: public LSA
    {
    public:
        PooMiddleOwn(Robot2017& robot);

        //Implements LSA
        virtual result execute(uint8_t nbItems = 0) override;
    };

    class PooMiddleOpp: public LSA
    {
    public:
        PooMiddleOpp(Robot2017& robot);

        //Implements LSA
        virtual result execute(uint8_t nbItems = 0) override;
    };

    class PooBorder: public LSA
    {
    public:
        PooBorder(Robot2017& robot);

        //Implements LSA
        virtual result execute(uint8_t nbItems = 0) override;
    };

    class PooBorderOpp: public LSA
    {
    public:
        PooBorderOpp(Robot2017& robot);

        //Implements LSA
        virtual result execute(uint8_t nbItems = 0) override;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_POO_H_ */
