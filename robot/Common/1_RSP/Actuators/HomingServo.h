/*
 * HomingServo.h
 *
 *  Created on: 28 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_ACTUATORS_HOMINGSERVO_H_
#define ROBOT_COMMON_1_RSP_ACTUATORS_HOMINGSERVO_H_

#include "RSP.h"

namespace ard
{
    /**
     * This class represents an assembly of a servo + one or 2 homing switches
     * The user is then driving the servo in a relative cmd coordinate system
     * 0 refers to the "min homing switch"
     * 1000 refers to the "max homing switch"
     */
    class HomingServo
    {
    public:
        HomingServo(FilteredInput* pinMin, FilteredInput* pinMax);

    protected:
        FilteredInput* pinMin;
        FilteredInput* pinMax;

        DelayUs perThousandToUs(uint16_t perThousand);
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_ACTUATORS_HOMINGSERVO_H_ */
