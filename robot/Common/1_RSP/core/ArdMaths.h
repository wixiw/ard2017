/*
 * ArdMaths.h
 *
 *  Created on: 9 nov. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ARDMATHS_H_
#define ROBOTS_ARDMATHS_H_

#include "WString.h"
#include <math.h>
#include "CommonMsg.pb.h"

namespace ard
{
    float
    moduloPiPi(float a);

    //Represent a geometric point in a 2D space
    class Point
    {
    public:
        float x;/*mm*/
        float y;/*mm*/

        virtual ~Point()
        {
        }
        ;
        Point();
        Point(float x /*mm*/, float y/*mm*/);

        //return the symetric from the x axis
        Point toAmbiPose(eColor c) const;

        //return the angle from the x-axis to the target point in rad
        float angleTo(Point p) const;

        //return the distance to the other point in mm
        float distanceTo(Point p) const;

        String virtual toString() const;

        //Get the associated proto struct
        apb_Point getProto() const;
    };

    //Represent a pose in a 2D space : Point + heading
    class PointCap: public Point
    {
    public:
        double h;/*rad*/

        PointCap();
        PointCap(Point p);
        PointCap(float x /*mm*/, float y/*mm*/, double h/*degree*/);

        //return the symetric from the x axis
        PointCap toAmbiPose(eColor c) const;

        String toString() const override;

        //Get the associated proto struct
        apb_PointCap getProto() const;

        //Build from the associated proto struct
        static PointCap fromProto(apb_PointCap const& p);
    };

} /* namespace ard */

#endif /* ROBOTS_ARDMATHS_H_ */
