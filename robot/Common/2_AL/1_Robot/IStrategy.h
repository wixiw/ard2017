/*
 * IStrategy.h
 *
 *  Created on: 26 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_0_STRATFWK_ISTRATEGY_H_
#define ROBOT_COMMON_2_AL_0_STRATFWK_ISTRATEGY_H_

#include "ArdOs.h"

namespace ard
{
    class IStrategy: public PolledObject
    {
    public:
        //Redefined to force the name
        IStrategy(String const& name):PolledObject(name){};

        virtual void start() = 0;
    };
}
#endif /* ROBOT_COMMON_2_AL_0_STRATFWK_ISTRATEGY_H_ */
