/*
 * ArdMaths.h
 *
 *  Created on: 9 nov. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ARDMATHS_H_
#define ROBOTS_ARDMATHS_H_

#include "ArdUtils.h"
#include "Types.pb.h"
#include <math.h>

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
        Point
        toAmbiPose(eColor c);

        //return the angle from the x-axis to the target point in rad
        float angleTo(Point p);

        //return the distance to the other point in mm
        float distanceTo(Point p);

        String
        virtual toString();
    };

    //Represent a pose in a 2D space : Point + heading
    class PointCap: public Point
    {
    public:
        float h;/*rad*/

        PointCap();
        PointCap(Point p);
        PointCap(float x /*mm*/, float y/*mm*/, float h/*degree*/);

        //return the symetric from the x axis
        PointCap
        toAmbiPose(eColor c);

        String
        toString() override;
    };

} /* namespace ard */

#endif /* ROBOTS_ARDMATHS_H_ */
