/*
 * Selftest.h
 *
 *  Created on: 18 mai 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_3_STRATEGIES_SELFTEST_H_
#define ROBOT_COMMON_2_AL_3_STRATEGIES_SELFTEST_H_

#include "../Robot.h"

namespace ard
{
    typedef enum
    {
        ST_IDLE,
		ST_OPEN_ARMS,
        ST_LIFTER_UP,
        ST_LIFTER_DOWN,
        ST_LIFTER_UP2,
        ST_TURN_WHEELS,
        ST_STOP_WHEELS,
        ST_RETRACT,
		ST_FUNNY_LAUNCH,
		ST_FUNNY_REST,
		ST_DONE,
    } eSTstate;

    class Selftest: public IStrategy
    {
    public:
        Selftest(Robot2017& robot);

        //Implements IStrategy
        void start();

        //Implements IStrategy
        void stop();

        //Implements IStrategy
        void update(TimeMs sinceLastCall) override;

    private:
        eSTstate state;
        Robot2017& robot;
        SwTimer chrono;
        SwTimer timeout;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_3_STRATEGIES_SELFTEST_H_ */
