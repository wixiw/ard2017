/*
 * LSAPoo.h
 *
 *  Created on: 13 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_LSA_POO_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_LSA_POO_H_

#include "StratInterfaces.h"

namespace ard
{

    class LSA_PooMiddleCenter: public LSA
    {
    public:
        LSA_PooMiddleCenter(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

    class LSA_PooMiddleOwn: public LSA
    {
    public:
        LSA_PooMiddleOwn(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

    class LSA_PooMiddleOpp: public LSA
    {
    public:
        LSA_PooMiddleOpp(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

    class LSA_PooBorder: public LSA
    {
    public:
        LSA_PooBorder(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

    class LSA_PooBorderOpp: public LSA
    {
    public:
        LSA_PooBorderOpp(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_LSA_POO_H_ */
