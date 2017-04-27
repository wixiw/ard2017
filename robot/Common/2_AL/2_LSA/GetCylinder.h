/*
 * GetCylinder.h
 *
 *  Created on: 16 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_GETCYLINDER_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_GETCYLINDER_H_

#include "StratFwk.h"
#ifdef BUILD_STRATEGY

namespace ard
{
    class GetCylinderCraters: public LSA
    {
    public:
        GetCylinderCraters(Robot2017& robot);

        //Implements LSA
        virtual LSAResult execute(uint8_t nbItems = 0) override;
    };
    
    class GetCylinderCorner: public LSA
    {
    public:
        GetCylinderCorner(Robot2017& robot);

        //Implements LSA
        virtual LSAResult execute(uint8_t nbItems = 0) override;
    };

    class GetCylinderOpp: public LSA
    {
    public:
        GetCylinderOpp(Robot2017& robot);

        //Implements LSA
        virtual LSAResult execute(uint8_t nbItems = 0) override;
    };

} /* namespace ard */

#endif
#endif /* ROBOT_COMMON_2_AL_STRATEGIES_GETCYLINDER_H_ */