/*
 * ArdMaths.cpp
 *
 *  Created on: 9 nov. 2016
 *      Author: wix
 */

#include "ArdMaths.h"
#include "ArdOs.h"

using namespace ard;

float
ard::moduloPiPi (float a)
{
  if (a <= -M_PI)
    return a + 2 * M_PI;
  else if (a > M_PI)
    return a - 2 * M_PI;
  else
    return a;
}

Point::Point () :
    x (0), y (0)
{
}
;

Point::Point (float _x, float _y):
  x (_x), y (_y)
{
}
;

Point
Point::toAmbiPose (color_t c)
{
  Point res;
  ardAssert (c != COLOR_UNDEF, "Color should not be undefined");
  if (c == COLOR_SYM)
    {
      res.x = -x;
      res.y = y;
    }
  else
    {
      res.x = x;
      res.y = y;
    }
  return res;
}

float Point::angleTo(Point p)
{
  return atan2 (p.y - y, p.x - x);
}

float Point::distanceTo(Point p)
{
  return sqrt( (p.y - y)*(p.y - y) + (p.x - x)*(p.x - x) );
}


String
Point::toString ()
{
  return String("(") + x + ", " + y + ")";
}




PointCap::PointCap () :
    Point(), h (0)
{
}
;

PointCap::PointCap (Point p) :
    Point(p.x, p.y), h (0)
{
}
;

PointCap::PointCap (float x, float y, float h) :
    Point(x, y), h (h)
{
}
;

PointCap
PointCap::toAmbiPose (color_t c)
{
  PointCap res = Point::toAmbiPose(c);
  if (c == COLOR_SYM)
      res.h = M_PI - h;
  else
      res.h = h;
  return res;
}

String
PointCap::toString ()
{
  return String("(") + x + ", " + y + ", " + h + ")";
}
