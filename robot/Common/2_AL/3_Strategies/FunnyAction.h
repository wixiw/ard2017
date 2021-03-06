/*
 * FunnyAction.h
 *
 *  Created on: 20 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_3_STRATEGIES_FUNNYACTION_H_
#define ROBOT_COMMON_2_AL_3_STRATEGIES_FUNNYACTION_H_

#include "../Robot.h"

namespace ard
{
    class FunnyAction: public IStrategy
    {
    public:
        FunnyAction(Robot2017& robot);

        //Implements IStrategy
        void start();
        
        //Implements IStrategy
        void stop(){};

        //Implements IStrategy
        void update(TimeMs sinceLastCall) override;

    private:
        DelayMs duration;
        Robot2017& robot;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_3_STRATEGIES_FUNNYACTION_H_ */
