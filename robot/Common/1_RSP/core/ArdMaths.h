/*
 * ArdMaths.h
 *
 *  Created on: 9 nov. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ARDMATHS_H_
#define ROBOTS_ARDMATHS_H_

#include "string/WString.h"
#include "arm_math.h"
#include "CommonMsg.pb.h"
#include "wiring_constants.h"

//Checks if a value is in a range
#define IS_IN_RANGE(min, value, max) if( min <= value && value <= max)
#define IS_OUT_RANGE(min, value, max) if( !(min <= value && value <= max))

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) ;

namespace ard
{
    /*rad*/ double moduloPiPi(double a /*rad*/);

    //Eventually add 180° to an angle representing the fact that you are facing rear
    /*rad*/ double headingToDir(double h /*rad*/, eDir sens);

    //Test if point (x,y) is in circle of center (xc, yc) and radius.
    bool isInCircle(float x, float y, float xc, float yc, float radius);

    //Represent a geometric point in a 2D space
    class Point
    {
    public:
        virtual ~Point()
        {
        }
        ;
        Point();
        Point(double x /*mm*/, double y/*mm*/);

        //return the symetric from the x axis
        Point toAmbiPoint(eColor c) const;

        //return the angle from the x-axis to the target point in rad
        double angleTo(Point p) const;

        //return the distance to the other point in mm
        double distanceTo(Point p) const;

        String virtual toString() const;

        //Get the associated proto struct
        apb_Point getProto() const;

        //Translate the point of the following distances in mm
        void translate(double dx, double dy);

        //Build from the associated proto struct
        static Point fromProto(apb_Point const& p);

        //Translate the point from the following angle and distance (degree and mm).
        void translatePolar(double dtheta, double dist);

        double x;/*mm*/
        double y;/*mm*/
    };

    //Represent a pose in a 2D space : Point + heading
    class Pose2D: public Point
    {
    public:
        Pose2D();
        Pose2D(Point p);
        Pose2D(double x /*mm*/, double y/*mm*/, double h/*degree*/);

        //return the symetric from the x axis
        Pose2D toAmbiPose(eColor c) const;

        String toString() const override;

        //Get the associated proto struct
        apb_PointCap getProto() const;

        //return the angle from the heading to the target point in rad
        double angleHeadingTo(Point p) const;

        //Build from the associated proto struct
        static Pose2D fromProto(apb_PointCap const& p);

        //accessor
        double hDegree() const {return degrees(h);};
        void hDegree(double newH){h = moduloPiPi(radians(newH));};

        double h;/*rad*/
    };

} /* namespace ard */

#endif /* ROBOTS_ARDMATHS_H_ */
