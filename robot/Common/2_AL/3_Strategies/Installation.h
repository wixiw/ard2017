/*
 * Installation.h
 *
 *  Created on: 18 mai 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_3_STRATEGIES_INSTALLATION_H_
#define ROBOT_COMMON_2_AL_3_STRATEGIES_INSTALLATION_H_

#include "../Robot.h"

namespace ard
{
    typedef enum
    {
        IP_IDLE,
        IP_RECAL_FLIP_FLOP,
        IP_RECAL_START_Y,
        IP_MOVE_A_BIT,
        IP_GO_TO_START_POS,
        IP_DONE
    } eIPstate;

    class InstallPen: public IStrategy
    {
    public:
        InstallPen(Robot2017& robot);

        //Implements IStrategy
        void start();

        //Implements IStrategy
        void stop();

        //Implements IStrategy
        void update(TimeMs sinceLastCall) override;

    private:
        eIPstate state;
        Robot2017& robot;
        SwTimer timeout;
    };

    typedef enum
    {
        IT_IDLE,
        IT_DONE
    } eITstate;

    class InstallTration: public IStrategy
    {
    public:
        InstallTration(Robot2017& robot);

        //Implements IStrategy
        void start();

        //Implements IStrategy
        void stop();

        //Implements IStrategy
        void update(TimeMs sinceLastCall) override;

    private:
        eITstate state;
        Robot2017& robot;
        SwTimer timeout;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_3_STRATEGIES_INSTALLATION_H_ */
