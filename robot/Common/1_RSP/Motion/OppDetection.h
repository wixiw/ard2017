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
        OppDetection();

        //Overrides RobotParametersListener
        void updateConf(RobotParameters* newConf) override;

        //Test if opponent is on robot path, calls isOpponentAhead() or isOpponentBehind()
        bool isOpponentOnPath(eDir direction, Pose2D const& robotPose);

        //Test if opponent is ahead of robot
        bool isOpponentAhead(Pose2D const& robotPose);

        //Test if opponent is behind robot
        bool isOpponentBehind(Pose2D const& robotPose);

        //Test if a waypoint is accessible.
        //The robot is considered as being a circle of radius robotSize in mm
        //Works only on prefered color
        bool isWaypointValid(Pose2D p, Distance robotSize);

        //Active/Deactivate avoidance system
        void enableAvoidance(bool on);

        //is true when robot is simulated
        bool simulated;

        //is true when a fake robot is simulated
        bool fakeRobot;

        //Avoidance sensors
        FilteredInput omronFront;
        FilteredInput omronRear;

    private:
    	//Return true if the detection point is a valid point, false if it is out of table or
        //in an area considered as always safe.
        //Works only on prefered color
        bool isDetectionValid(Pose2D p, Distance wallExtension);
        
        bool avoidanceActive; //is true when avoidance system is active
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_OPPDETECTION_H_ */
