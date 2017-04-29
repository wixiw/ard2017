/*
 * HomingServo.cpp
 *
 *  Created on: 28 avr. 2017
 *      Author: wix
 */

#include "HomingServo.h"

namespace ard
{

    HomingServo::HomingServo(FilteredInput* pinMin, FilteredInput* pinMax):
    pinMin(pinMin),
    pinMax(pinMax)
    {
        //At least one pin shall be non null
        ASSERT(pinMin != NULL || pinMax != NULL);
    }

} /* namespace ard */
