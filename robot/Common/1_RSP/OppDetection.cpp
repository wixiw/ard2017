/*
 * OppDetection.cpp
 *
 *  Created on: 8 mai 2017
 *      Author: wix
 */

#include "OppDetection.h"
#include "Log.h"
#include "RobotParameters.h"

using namespace ard;

OppDetection::OppDetection(uint16_t safetyArea):
        simulated(false),
        fakeRobot(false),
        color(eColor_PREF),
        omronFront(OMRON1, 50, 1000),
        omronRear(OMRON3, 50, 1000),
        safetyArea(safetyArea),
        avoidanceActive(false),//start desactivated, so that activation is done with start, ensuring avoidance is unactivated in simulation
        conf(NULL)
{
}

void OppDetection::updateConf(RobotParameters* newConf)
{
    ASSERT(newConf);
    conf = newConf;

    omronFront.setDebounceLow(newConf->detectionWaitForOppMove());
    omronRear.setDebounceLow(newConf->detectionWaitForOppMove());
}

void OppDetection::setColor(eColor c)
{
    ASSERT_TEXT(c != eColor_UNKNOWN, "color should not be set to undefined.");
    color = c;
}

bool OppDetection::isOpponentOnPath(eDir direction, PointCap& robotPose)
{
    switch(direction)
    {
    case eDir_FORWARD:
        return isOpponentAhead(robotPose);
        break;
    case eDir_BACKWARD:
        return isOpponentBehind(robotPose);
        break;
    case eDir_UNDEFINED:
    default:
        ASSERT_TEXT(false, "unexpected default case");
        return false;
        break;
    }
}

bool OppDetection::isOpponentAhead(PointCap robotPose)
{
    bool opponentPresent = false;

    if(   (simulated && fakeRobot)
      || (!simulated && omronFront.read()) == GPIO_HIGH)
    {
        robotPose.translatePolar(robotPose.hDegree(), conf->xav() + conf->deccDist());
        opponentPresent |= isDetectionValid(robotPose);
    }

//    if( opponentPresent )
//        LOG_DEBUG(String("Opponent ahead : read=") + omronFront.read() + " raw=" + omronFront.readRaw() + " pose=" + detectedPose.toString() + " simu="+simulated+ " fake=" + fakeRobot);
//    else
//        LOG_DEBUG("No opponent ahead.");

    return opponentPresent;
}

bool OppDetection::isOpponentBehind(PointCap robotPose)
{
    bool opponentPresent = false;

    if(   (simulated && fakeRobot)
      || (!simulated && omronRear.read()) == GPIO_HIGH)
    {
        robotPose.translatePolar(robotPose.hDegree(), - conf->xar() - conf->deccDist());
        opponentPresent |= isDetectionValid(robotPose);
    }

//    if( opponentPresent )
//        LOG_DEBUG(String("Opponent behind : read=") + omronRear.read() + " raw=" + omronRear.readRaw() + " pose=" + detectedPose.toString() + " simu="+simulated+ " fake=" + fakeRobot);
//    else
//        LOG_DEBUG("No opponent ahead.");

    return opponentPresent;
}

void OppDetection::enableAvoidance(bool on)
{
    if( on && !avoidanceActive )
    {
        LOG_INFO("Avoidance system activated.");
        omronFront.reset();
        omronRear.reset();
    }
    if( !on && avoidanceActive )
        LOG_INFO("Avoidance system deactivated.");

    avoidanceActive = on;
}

bool OppDetection::isDetectionValid(PointCap p)
{
    bool isOnTable = true;

    IS_OUT_RANGE(-TABLE_BORDER_X + SAFETY_AREA, p.x, TABLE_BORDER_X - SAFETY_AREA)
        isOnTable = false;
    IS_OUT_RANGE(-TABLE_TOP_Y + SAFETY_AREA, p.y, TABLE_TOP_Y - SAFETY_AREA)
        isOnTable = false;

    //If detected point is in Yellow start area, behind the wall : ignore it.
    if( 790 < p.x && 618 - SAFETY_AREA < p.y && 10 < fabs(moduloPiPi(p.hDegree()-180)) )
        isOnTable = false;
    //If detected point is in Blue start area, behind the wall : ignore it.
    if( p.x < -790 && 618 - SAFETY_AREA < p.y && 10 < fabs(p.hDegree()) )
        isOnTable = false;

    //Exclude Yellow start opened area (safety area not count to prevent robot to ignore opponent when going out of start pos)
    if( color == eColor_PREF && 430 < p.x && 618 < p.y)
        isOnTable = false;
    //Exclude Blue start opened area (safety area not count to prevent robot to ignore opponent when going out of start pos)
    if( color == eColor_SYM && p.x < -430  && 618 < p.y )
        isOnTable = false;

    return isOnTable;
}
