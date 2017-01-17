#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include "BSP.h"
#include "ArdOs.h"

namespace ard
{
    class ObjectColor
    {
    public:
        unsigned short r; // red
        unsigned short g; // green
        unsigned short b; // blue
        unsigned short l; // luminance

        ObjectColor():r(0),g(0),b(0),l(0){};

        void computeFromRawData(uint16_t r,uint16_t g,uint16_t b,uint16_t c);

        String toString();
    };

    /**
     * This class manage the color sensor module, base on the Adafruit TCS34725 chip
     */
    class ColorSensor: public PolledObject
    {
    public:
        ColorSensor();
        ~ColorSensor();

        //Implements PolledObject: read the capteur data and filters outputs to get the measured value
        void update(TimeMs sinceLastCall) override;

    private:
        Adafruit_TCS34725 tcs;
        ObjectColor color;
    };
}    //end namespace

#endif