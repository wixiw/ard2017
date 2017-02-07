#include "ColorSensor.h"

#include "Log.h"

using namespace ard;

/*
 * Normalise componentes (depends on integration times) and compute red, green and blue
 * Also, give an indication of luminosity (quantity of light received)
 */
void
ObjectColor::computeFromRawData(uint16_t red, uint16_t green, uint16_t blue, uint16_t lux)
{
    r = map(red,0,lux,0,255);
    g = map(green,0,lux,0,255);
    b = map(blue,0,lux,0,255);
    l = map(lux,0,45000,0,255); // 45000 is heuristic

    // compute specific colors
    if (       100 <= r && r <= 135
            && 93  <= g && g <= 109
            && 38  <= b && b <= 50 )
        c = objectColor_YELLOW;
    else if (  34 <= r && r <= 82
            && 83  <= g && g <= 90
            && 105  <= b && b <= 135 )
        c = objectColor_BLUE;
    else
        c = objectColor_UNDEF;

}

String
ObjectColor::toString()
{
    String color = "";
    switch (c){
        case (objectColor_UNDEF)     : color = "UNDEFINED"; break;
        case (objectColor_BLUE)      : color = "BLUE"; break;
        case (objectColor_YELLOW)    : color = "YELLOW"; break;
        case (objectColor_WHITE)     : color = "WHITE"; break;
    default:;
    }

    return String("R:") + r + " G:" + g + " B:" + b + " L:" + l + " COLOR=>" + color;
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
    uint16_t r, g, b, lux; //, colorTemp, lux;
    tcs.getRawData(&r, &g, &b, &lux);
    //    colorTemp = tcs.calculateColorTemperature(r, g, b);
    //    lux = tcs.calculateLux(r, g, b);

    color.computeFromRawData(r,g,b,lux);
    //LOG_INFO(color.toString());
}

apb_ColorSensorState
ColorSensor::getState()
{
    apb_ColorSensorState state = apb_ColorSensorState_init_default;

    state.r = color.r;
    state.g = color.g;
    state.b = color.b;
    state.l = color.l;
    state.color = color.c;

    return state;
}

