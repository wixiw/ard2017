/*
 * BlinkLEDWrapper.h
 *
 *  Created on: 17 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_UT_YAKINDU_BLINKLEDWRAPPER_H_
#define ROBOT_UT_YAKINDU_BLINKLEDWRAPPER_H_

#include "BlinkLED.h"

namespace ard
{

    class BlinkLEDWrapper: public BlinkLED::SCI_BSP_OCB
    {
    public:
        BlinkLEDWrapper() = default;

        void setLed(sc_integer on) override;
    };

} /* namespace ard */

#endif /* ROBOT_UT_YAKINDU_BLINKLEDWRAPPER_H_ */
