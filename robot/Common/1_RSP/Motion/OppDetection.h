/*
 * OppDetection.h
 *
 *  Created on: 8 mai 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_OPPDETECTION_H_
#define ROBOT_COMMON_1_RSP_OPPDETECTION_H_

#include "Types.pb.h"
#include "BSP.hpp"
#include "core/ArdMaths.h"
#include "RobotParameters.h"

namespace ard
{
    class OppDetection: public RobotParametersListener
    {
    public:
        /**
         * @param safetyArea : the distance in mm from the table walls where we always supposed they won't be any opponent.
         *                  The higher the less we risk to detect the crocodile dancer, but it's impossible to see an opponent
         *                  which stick to the wall with a width of less than the safetyArea.
         */
        OppDetection(uint16_t safetyArea);

        //Overrides RobotParametersListener
        void updateConf(RobotParameters* newConf) override;

        //set match color
        void setColor(eColor c);

        //Test if opponent is on robot path, calls isOpponentAhead() or isOpponentBehind()
        bool isOpponentOnPath(eDir direction, Pose2D const& robotPose);

        //Test if opponent is ahead of robot
        bool isOpponentAhead(Pose2D const& robotPose);

        //Test if opponent is behind robot
        bool isOpponentBehind(Pose2D const& robotPose);

        //Active/Deactivate avoidance system
        void enableAvoidance(bool on);

        //is true when robot is simulated
        bool simulated;

        //is true when a fake robot is simulated
        bool fakeRobot;

        //match color
        eColor color;

        //Avoidance sensors
        FilteredInput omronFront;
        FilteredInput omronRear;

    private:
        //Return true if the detection point is a valid point, false if it is out of table or
        //in an area considered as always safe.
        bool isDetectionValid(Pose2D p);

        uint16_t safetyArea;
        bool avoidanceActive; //is true when avoidance system is active
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_OPPDETECTION_H_ */
