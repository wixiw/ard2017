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
    class InstallPen: public IStrategy
    {
        typedef enum
        {
            IP_IDLE,
			IP_PREPARE_FLIPFLOP,
            IP_RECAL_FLIP_FLOP,
            IP_RECAL_REFEREE_Y,
            IP_ESCAPE_REFEREE,
            IP_GO_TO_START_POS,
            IP_DONE
        } eIPstate;

    public:
        InstallPen(Robot2017& robot);

        //Implements IStrategy
        void start();

        //Implements IStrategy
        void stop();

        //Implements IStrategy
        void update(TimeMs sinceLastCall) override;

        //Override IStrategy
        virtual Pose2D getStartPosition(){return PEN_START_POSE;}

    private:
        eIPstate state;
        Robot2017& robot;
        SwTimer timeout;
    };

    class InstallTration: public IStrategy
    {
        typedef enum
        {
            IT_IDLE,
			IP_RECAL_START_Y,
			IT_MOVE_A_BIT,
			IT_RECAL_FLIP_FLOP,
			IT_ESCAPING_FOR_PEN,
			IT_ESCAPE_FOR_PEN,
            IT_WAIT_START,
			IT_WAIT_START_WITHDRAW,
            IT_GO_TO_START_POS,
            IT_DONE
        } eITstate;

    public:
        InstallTration(Robot2017& robot);

        //Implements IStrategy
        void start();

        //Implements IStrategy
        void stop();

        //Implements IStrategy
        void update(TimeMs sinceLastCall) override;

        //Override IStrategy
        virtual Pose2D getStartPosition(){return TRATION_START_POSE;}

    private:
        eITstate state;
        Robot2017& robot;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_3_STRATEGIES_INSTALLATION_H_ */
