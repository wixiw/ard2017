#include "ColorSensor.h"

#include "Log.h"

using namespace ard;

/*
 * Normalise componentes (depends on integration times) and compute red, green and blue
 * Also, give an indication of luminosity (quantity of light received)
 */
void
ObjectColor::computeFromRawData(uint16_t red, uint16_t green, uint16_t blue, uint16_t c)
{
    r = map(red,0,c,0,255);
    g = map(green,0,c,0,255);
    b = map(blue,0,c,0,255);
    l = map(c,0,45000,0,255); // 45000 is heuristic
}

String
ObjectColor::toString()
{
    return String("R:") + r + " G:" + g + " B:" + b + " L:" + l;
}

ColorSensor::ColorSensor()
: tcs(TCS34725_INTEGRATIONTIME_101MS)
{
}

ColorSensor::~ColorSensor()
{
}

void
ColorSensor::update(TimeMs sinceLastCall)
{
    uint16_t r, g, b, c; //, colorTemp, lux;
    tcs.getRawData(&r, &g, &b, &c);
//    colorTemp = tcs.calculateColorTemperature(r, g, b);
//    lux = tcs.calculateLux(r, g, b);

    color.computeFromRawData(r,g,b,c);
//    LOG_INFO(color.toString());
}

