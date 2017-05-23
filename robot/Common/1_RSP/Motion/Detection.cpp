/*
 * OppDetection.cpp
 *
 *  Created on: 8 mai 2017
 *      Author: wix
 */

#include "Detection.h"

#include "Log.h"

using namespace ard;

//This defines helps to remind that debounce values are retrieved from the configuration
#define OVERRIDEN_BY_CONFIG 1000

Detection::Detection():
        simulated(true),
        fakeRobot(false),
        omronFrontLeft(OMRON1, 50, OVERRIDEN_BY_CONFIG, true),
		omronFrontRight(OMRON2, 50, OVERRIDEN_BY_CONFIG, true),
		omronRearLeft(OMRON3, 50, OVERRIDEN_BY_CONFIG, true),
		omronRearRight(OMRON4, 50, OVERRIDEN_BY_CONFIG, true),
		omronLatLeft(OMRON1 /*TODO*/, 50, OVERRIDEN_BY_CONFIG, true),
		omronLatRight(OMRON1 /*TODO*/, 50, OVERRIDEN_BY_CONFIG, true),
		omronScan(OMRON3, 20, 20, true),
        avoidanceActive(false)//start desactivated, so that activation is done with start, ensuring avoidance is unactivated in simulation
{
}

void Detection::updateConf(RobotParameters* newConf)
{
	RobotParametersListener::updateConf(newConf);
    omronFrontLeft.setDebounceLow(newConf->detectionWaitForOppMove());
    omronFrontRight.setDebounceLow(newConf->detectionWaitForOppMove());
    omronRearLeft.setDebounceLow(newConf->detectionWaitForOppMove());
	omronRearRight.setDebounceLow(newConf->detectionWaitForOppMove());
	omronLatLeft.setDebounceLow(newConf->detectionWaitForOppMove());
	omronLatRight.setDebounceLow(newConf->detectionWaitForOppMove());
}

bool Detection::isOpponentOnPath(eDir direction, Pose2D const& robotPose)
{
	if(!avoidanceActive)
		return false;

    switch(direction)
    {
    case eDir_FORWARD:
        return isOpponentAhead(robotPose);
        break;
    case eDir_BACKWARD:
        return isOpponentBehind(robotPose);
        break;
    case eDir_BEST:
    default:
        ASSERT_TEXT(false, "unexpected default case");
        return false;
        break;
    }
}

bool Detection::isOpponentAhead(Pose2D const& robotPose)
{
	ASSERT_CONFIGURED();
	Pose2D pose = robotPose;

	if(!avoidanceActive)
		return false;

    if(   (simulated && fakeRobot)
      || (!simulated && omronFrontLeft.read() == GPIO_HIGH && omronFrontRight.read() == GPIO_HIGH) )
    {
    	pose.translatePolar(robotPose.hDegree(), conf->xav() + conf->avoidanceDistanceFront());
        return isDetectionValid(robotPose, SAFETY_AREA);
    }
    else
    {
    	//opponent
    	return false;
    }

//    if( opponentPresent )
//        LOG_DEBUG(String("Opponent ahead : read=") + omronFront.read() + " raw=" + omronFront.readRaw() + " pose=" + detectedPose.toString() + " simu="+simulated+ " fake=" + fakeRobot);
//    else
//        LOG_DEBUG("No opponent ahead.");
}

bool Detection::isOpponentBehind(Pose2D const& robotPose)
{
	ASSERT_CONFIGURED();
	Pose2D pose = robotPose;

	if(!avoidanceActive)
		return false;

    if(   (simulated && fakeRobot)
      || (!simulated && omronRearLeft.read() == GPIO_HIGH && omronRearRight.read() == GPIO_HIGH))
    {
        pose.translatePolar(robotPose.hDegree(), - conf->xar() - conf->avoidanceDistanceRear());
        return isDetectionValid(robotPose, SAFETY_AREA);
    }
    else
    {
    	//opponent
    	return false;
    }

//    if( opponentPresent )
//        LOG_DEBUG(String("Opponent behind : read=") + omronRear.read() + " raw=" + omronRear.readRaw() + " pose=" + detectedPose.toString() + " simu="+simulated+ " fake=" + fakeRobot);
//    else
//        LOG_DEBUG("No opponent ahead.");
}

bool Detection::isOpponentOnLeft(Pose2D const& robotPose)
{
	NOT_IMPLEMENTED();
	return false;
}

bool Detection::isOpponentOnRight(Pose2D const& robotPose)
{
	NOT_IMPLEMENTED();
	return false;
}

void Detection::enableAvoidance(bool on)
{
    if( on && !avoidanceActive )
    {
        LOG_INFO("[Detection] Avoidance system activated.");
        omronFrontLeft.reset();
        omronFrontRight.reset();
        omronRearLeft.reset();
        omronRearRight.reset();
        omronLatLeft.reset();
        omronLatRight.reset();
    }
    if( !on && avoidanceActive )
        LOG_INFO("[Detection] Avoidance system deactivated.");

    avoidanceActive = on;
}

bool Detection::isWaypointValid(Pose2D p, Distance robotSize)
{
	//Test if point is on table (border 1 and 5 excluded, hence bicolor dispenser are excluded too)
    IS_OUT_RANGE(-1420 + robotSize, p.x, 1420 - robotSize)
	{
    	LOG_INFO("[Detection] waypoint invalid because : x out of table");
        return false;
	}
    IS_OUT_RANGE(-TABLE_BORDER_Y + robotSize, p.y, TABLE_BORDER_Y - robotSize)
	{
    	LOG_INFO("[Detection] waypoint invalid because : y out of table");
        return false;
	}

    //Middle poo area
	if( isInCircle(p.x, p.y, 0, -TABLE_BORDER_Y, 800 + robotSize) )
	{
    	LOG_INFO("[Detection] waypoint invalid because : in middle poo area");
        return false;
	}

    //As an optimisation, some tests are only made if x is in the right table section

    //Left table half
    if( p.x < 0 )
    {
    	if( p.y < 0 )
    	{
			//Bottom left crater
			if( isInCircle(p.x, p.y, -430, -870, 125 + robotSize) )
			{
		    	LOG_INFO("[Detection] waypoint invalid because : in bottom left crater");
		        return false;
			}

			//Corner left crater
			if( isInCircle(p.x, p.y, -TABLE_BORDER_X, -TABLE_BORDER_Y, 540 + robotSize) )
			{
		    	LOG_INFO("[Detection] waypoint invalid because : in corner left crater");
		        return false;
			}

			//Left bicolor dispenser : already tested by lateral border
    	}
    	else
    	{
    		//Exclude opponent area
            if( p.x < -430 + robotSize && 618 - robotSize < p.y)
        	{
            	LOG_INFO("[Detection] waypoint invalid because : in opponent start area");
                return false;
        	}

			//Opp Top monocolor dispenser
			if( isInCircle(p.x, p.y, -350,  960, 80 + robotSize) )
			{
		    	LOG_INFO("[Detection] waypoint invalid because : in opp top monocolor dispenser");
		        return false;
			}

        	//Top left crater
        	if( isInCircle(p.x, p.y, -850,  460, 125 + robotSize) )
        	{
            	LOG_INFO("[Detection] waypoint invalid because : in top left crater");
                return false;
        	}

        	//Right bicolor dispenser : already tested by lateral border
    	}
    }
    //Right table half
    else
    {
    	if( p.y < 0 )
    	{
			//Bottom right crater
			if( isInCircle(p.x, p.y, 430, -870, 125 + robotSize) )
			{
		    	LOG_INFO("[Detection] waypoint invalid because : in bottom right crater");
		        return false;
			}

			//Corner right crater
			if( isInCircle(p.x, p.y, TABLE_BORDER_X, -TABLE_BORDER_Y, 540 + robotSize) )
			{
		    	LOG_INFO("[Detection] waypoint invalid because : in corner right crater");
		        return false;
			}
    	}
    	else
    	{
            //Exclude some of our start area
            if( 790 - robotSize < p.x && 618 - robotSize < p.y)
        	{
            	LOG_INFO("[Detection] waypoint invalid because : in our start area");
                return false;
        	}

			//Our top monocolor dispenser
			if( isInCircle(p.x, p.y, 350,  960, 80 + robotSize) )
			{
		    	LOG_INFO("[Detection] waypoint invalid because : in our top monocolor dispenser");
		        return false;
			}

			//Top right crater
			if( isInCircle(p.x, p.y, 850,  460, 125 + robotSize) )
			{
		    	LOG_INFO("[Detection] waypoint invalid because : in top right crater");
		        return false;
			}
    	}
    }

    //If we reach here, then the point is valid.
    LOG_INFO("[Detection] waypoint valid, hell yeah !");
    return true;
}


bool Detection::isDetectionValid(Pose2D p, Distance wallExtension)
{
    IS_OUT_RANGE(-TABLE_BORDER_X + wallExtension, p.x, TABLE_BORDER_X - wallExtension)
        return false;
    IS_OUT_RANGE(-TABLE_BORDER_Y + wallExtension, p.y, TABLE_BORDER_Y - wallExtension)
        return false;

    //Exclude Yellow start opened area (safety area not count to prevent robot to ignore opponent when going out of start pos)
    if( 430 < p.x && 618 < p.y)
        return false;

    //If detected point is in Yellow start area, behind the wall : ignore it.
    if( 790 < p.x && 618 - wallExtension < p.y && 10 < fabs(moduloPiPi(p.hDegree()-180)) )
        return false;

    //If we reach here, then the point is valid.
    return true;
}
