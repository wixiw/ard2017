/*
 * ArdMaths.cpp
 *
 *  Created on: 9 nov. 2016
 *      Author: wix
 */

#include "ArdMaths.h"
#include "ArdFramework.h"
#include "wiring_constants.h"

using namespace ard;

double ard::moduloPiPi(double a)
{
    if (a <= -M_PI)
        return a + 2 * M_PI;
    else if (a > M_PI)
        return a - 2 * M_PI;
    else
        return a;
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

Point Point::toAmbiPose(eColor c) const
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
        : Point(x, y), h(h*DEG_TO_RAD)
{
}
;

PointCap PointCap::toAmbiPose(eColor c) const
{
    PointCap res = Point::toAmbiPose(c);
    if (c == eColor_SYM)
        res.h = moduloPiPi(M_PI - h);
    else
        res.h = h;
    return res;
}

String PointCap::toString() const
{
    return String("(") + x + "mm, " + y + "mm, " + h + "rad), h=" + RAD_TO_DEG*h +"°";
}

apb_PointCap PointCap::getProto() const
{
    apb_PointCap p = apb_PointCap_init_zero;
    p.x = x;
    p.y = y;
    p.h = h;
    return p;
}

PointCap PointCap::fromProto(apb_PointCap const& p)
{
    PointCap res;
    res.x = p.x;
    res.y = p.y;
    res.h = p.h;
    return res;
}
