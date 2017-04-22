/*
 * ArdMaths.h
 *
 *  Created on: 9 nov. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ARDMATHS_H_
#define ROBOTS_ARDMATHS_H_

#include "string/WString.h"
#include <math.h>
#include "CommonMsg.pb.h"
#include "wiring_constants.h"

//Checks if a value is in a range
#define IS_IN_RANGE(min, value, max) if( min <= value && value <= max)
#define IS_OUT_RANGE(min, value, max) if( !(min <= value && value <= max))

namespace ard
{
    double
    moduloPiPi(double a);

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
        Point toAmbiPose(eColor c) const;

        //return the angle from the x-axis to the target point in rad
        double angleTo(Point p) const;

        //return the distance to the other point in mm
        double distanceTo(Point p) const;

        String virtual toString() const;

        //Get the associated proto struct
        apb_Point getProto() const;

        //Translate the point of the following distances in mm
        void translate(double dx, double dy);

        //Translate the point from the following angle and distance (degree and mm).
        void translatePolar(double dtheta, double dist);

        double x;/*mm*/
        double y;/*mm*/
    };

    //Represent a pose in a 2D space : Point + heading
    class PointCap: public Point
    {
    public:
        PointCap();
        PointCap(Point p);
        PointCap(double x /*mm*/, double y/*mm*/, double h/*degree*/);

        //return the symetric from the x axis
        PointCap toAmbiPose(eColor c) const;

        String toString() const override;

        //Get the associated proto struct
        apb_PointCap getProto() const;

        //Build from the associated proto struct
        static PointCap fromProto(apb_PointCap const& p);

        //accessor
        double hDegree() const {return degrees(h);};
        void hDegree(double newH){h = radians(newH);};

        double h;/*rad*/
    };

} /* namespace ard */

#endif /* ROBOTS_ARDMATHS_H_ */
