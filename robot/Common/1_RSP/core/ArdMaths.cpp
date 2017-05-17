/*
 * ArdMaths.cpp
 *
 *  Created on: 9 nov. 2016
 *      Author: wix
 */

#include "ArdMaths.h"
#include "ArdFramework.h"

using namespace ard;

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double ard::moduloPiPi(double a)
{
    if (a <= -M_PI)
        return a + 2 * M_PI;
    else if (a > M_PI)
        return a - 2 * M_PI;
    else
        return a;
}

double ard::headingToDir(double h, eDir sens)
{
    switch (sens) {
        case eDir_FORWARD:
            return moduloPiPi(h);
            break;
        case eDir_BACKWARD:
            return moduloPiPi(h+M_PI);
            break;
        default:
            ASSERT(false);
            return 0;
            break;
    }
}

Point::Point()
        : x(0), y(0)
{
}
;

Point::Point(double _x, double _y)
        : x(_x), y(_y)
{
}
;

Point Point::toAmbiPoint(eColor c) const
{
    Point res;
    ASSERT_TEXT(c != eColor_UNKNOWN, "Color should not be undefined");
    if (c == eColor_SYM)
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

double Point::angleTo(Point p) const
{
    return atan2(p.y - y, p.x - x);
}

double Point::distanceTo(Point p) const
{
    return sqrt((p.y - y) * (p.y - y) + (p.x - x) * (p.x - x));
}

String Point::toString() const
{
    return String("(") + x + "mm, " + y + "mm)";
}

apb_Point Point::getProto() const
{
    apb_Point p = apb_Point_init_zero;
    p.x = x;
    p.y = y;
    return p;
}

void Point::translate(double dx, double dy)
{
    x += dx;
    y += dy;
}

void Point::translatePolar(double dtheta, double dist)
{
    x += dist * cos( radians(dtheta) );
    y += dist * sin( radians(dtheta) );
}

Point Point::fromProto(apb_Point const& p)
{
    Point res;
    res.x = p.x;
    res.y = p.y;
    return res;
}

/**
 * ----------------------------------------------------------------------------
 */

PointCap::PointCap()
        : Point(), h(0)
{
}
;

PointCap::PointCap(Point p)
        : Point(p.x, p.y), h(0)
{
}
;

PointCap::PointCap(double x, double y, double h)
        : Point(x, y), h(moduloPiPi(h*DEG_TO_RAD))
{
}
;

double PointCap::angleHeadingTo(Point p) const
{
    return moduloPiPi(angleTo(p) - h);
}

PointCap PointCap::toAmbiPose(eColor c) const
{
    PointCap res = Point::toAmbiPoint(c);
    if (c == eColor_SYM)
        res.h = moduloPiPi(M_PI - h);
    else
        res.h = h;
    return res;
}

String PointCap::toString() const
{
    return String("(") + x + "mm, " + y + "mm, " + RAD_TO_DEG*h + "°), h=" + RAD_TO_DEG*h +" rad";
}

apb_PointCap PointCap::getProto() const
{
    apb_PointCap p = apb_PointCap_init_zero;
    p.x = x;
    p.y = y;
    p.h = degrees(h);
    return p;
}

PointCap PointCap::fromProto(apb_PointCap const& p)
{
    PointCap res;
    res.x = p.x;
    res.y = p.y;
    res.h = radians(p.h);
    return res;
}

