#include <Arduino.h>
#include "LogThread.h"
#include "ColorSensor.h"
#include "BSP.h"

using namespace ard;

ColorSensor::ColorSensor()
{
  tcs = new Adafruit_TCS34725();
}

ColorSensor::~ColorSensor()
{
  if (tcs) delete tcs;
}

void
ColorSensor::init()
{
  connected = (tcs && tcs->begin());
}

void
ColorSensor::update (TimeMs sinceLastCall)
{
  static bool logOnce = false;
  if (!logOnce)
  {
    logOnce = true;
    if (connected)
    {
      LOG_INFO("Color sensor found.");
    }
    else
    {
      LOG_ERROR("No TCS34725 (color sensor) found ... check your connections");
    }
  }
  
//   if (connected)
//   {
//     uint16_t r, g, b, c, colorTemp, lux;
//     tcs->getRawData(&r, &g, &b, &c);
//     colorTemp = tcs->calculateColorTemperature(r, g, b);
//     lux = tcs->calculateLux(r, g, b);
//     //LOG_INFO(String("R:") + r + " G:" + g + " B:" + b + " Lux:" + lux + " temp:" + colorTemp);
//   }
}

