/*
 * ArdMaths.h
 *
 *  Created on: 9 nov. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ARDMATHS_H_
#define ROBOTS_ARDMATHS_H_

#include "ArdUtils.h"

namespace ard
{

  float
  moduloPiPi (float a);

  //Represent a geometric point in a 2D space
  class Point
  {
  public:
    float x;
    float y;

    virtual ~Point(){};
    Point ();

    //return the symetric from the x axis
    virtual Point
    toAmbiPose (color_t c);

    //return the angle from the x-axis to the target point
    float angleTo(Point p);

    //return the distance to the other point
    float distanceTo(Point p);

    String
    virtual toString ();
  };

  //Represent a pose in a 2D space : Point + heading
  class PointCap : public Point
  {
  public:
    float h;

    PointCap ();
    PointCap (Point p);

    //return the symetric from the x axis
    PointCap
    toAmbiPose (color_t c);

    String
    toString ();
  };

} /* namespace ard */

#endif /* ROBOTS_ARDMATHS_H_ */
