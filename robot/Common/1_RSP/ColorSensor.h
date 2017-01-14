// Classe de gestion de la navigation (déplacements du robot sur la table)
#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include "BSP.h"
#include "Types.pb.h"
#include "Adafruit_TCS34725.h"

namespace ard
{
  /**
  * This class manage the color sensor module, base on the Adafruit TCS34725 chip
  */
  class ColorSensor : public IMiniPeriodicThread
  {
    public:
    ColorSensor();
    ~ColorSensor();

    //Implements IMiniThread
    void
    init() override;
		void
		update (TimeMs sinceLastCall) override;

    private:
    Adafruit_TCS34725* tcs;
    bool connected;
  };
}    //end namespace

#endif
