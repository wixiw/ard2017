/*
 * FunnyAction.h
 *
 *  Created on: 20 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_3_STRATEGIES_FUNNYACTION_H_
#define ROBOT_COMMON_2_AL_3_STRATEGIES_FUNNYACTION_H_

#include "Robot2017.h"

namespace ard
{
    class FunnyAction: public IStrategy
    {
    public:
        FunnyAction(Robot2017& robot);
<<<<<<< HEAD

        //Implements IStrategy
        void start();
=======
>>>>>>> 47c8f5df3f245d1a6d1a376ebf746dc9ee2e0a2c

        //Implements IStrategy
        void update(TimeMs sinceLastCall) override;

    private:
        DelayMs duration;
        Robot2017& robot;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_3_STRATEGIES_FUNNYACTION_H_ */
