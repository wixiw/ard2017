/*
 * FunnyAction.h
 *
 *  Created on: 20 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_3_STRATEGIES_FUNNYACTION_H_
#define ROBOT_COMMON_2_AL_3_STRATEGIES_FUNNYACTION_H_

#include "0_Strategy2017.h"

#ifdef BUILD_STRATEGY

namespace ard
{
    class Robot2017;

    class FunnyAction: public Strategy2017
    {
    public:
        FunnyAction(Robot2017* robot);

        //Implements IStrategy
        void update(TimeMs sinceLastCall) override;

    private:
        DelayMs duration;
    };

} /* namespace ard */

#endif //BUILD_STRATEGY

#endif /* ROBOT_COMMON_2_AL_3_STRATEGIES_FUNNYACTION_H_ */
