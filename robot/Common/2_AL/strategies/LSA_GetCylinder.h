/*
 * LSA_GetCylinder.h
 *
 *  Created on: 16 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_STRATEGIES_LSA_GETCYLINDER_H_
#define ROBOT_COMMON_2_AL_STRATEGIES_LSA_GETCYLINDER_H_

#include "StratInterfaces.h"

namespace ard
{
    class LSA_GetCylinderCraters: public LSA
    {
    public:
        LSA_GetCylinderCraters(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };
    
    class LSA_GetCylinderCorner: public LSA
    {
    public:
        LSA_GetCylinderCorner(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

    class LSA_GetCylinderOpp: public LSA
    {
    public:
        LSA_GetCylinderOpp(Robot2017& robot);

        //Implements LSA
        virtual LSA_result execute(uint8_t nbItems = 0) override;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_STRATEGIES_LSA_GETCYLINDER_H_ */
