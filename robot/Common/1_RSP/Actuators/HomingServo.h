/*
 * HomingServo.h
 *
 *  Created on: 28 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_ACTUATORS_HOMINGSERVO_H_
#define ROBOT_COMMON_1_RSP_ACTUATORS_HOMINGSERVO_H_

#include "AccelServo.h"

namespace ard
{
    typedef enum
    {
        IDLE,
        HOMING_MIN,
        HOMING_MAX,
        MOVING,
    } eHSState;

    class FilteredInput;

    /**
     * This class represents an assembly of a servo + one or 2 homing switches
     * The user is then driving the servo in a relative cmd coordinate system
     * 0 refers to the "min homing switch"
     * 1000 refers to the "max homing switch"
     */
    class HomingServo: public AccelServo
    {
    public:
        /**
         * @param pinMin : is the min homing switch (position 0)
         * @param pinMax : is the max homing switch (position 1000)
         * @param inverted : set to true if clockwise direction goest from max to min
         */
        HomingServo(String const& name, int servoOutPin, FilteredInput* pinMin, FilteredInput* pinMax, bool inverted = false);

        //Implements PolledObject
        void update(DelayMs sinceLastCall) override;

        //Override AccelServo
        virtual void goTo(uint16_t value) override;

        //Override AccelServo
        virtual bool isTargetReached() override;

    protected:
        FilteredInput* pinMin;
        FilteredInput* pinMax;
        eHSState state;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_ACTUATORS_HOMINGSERVO_H_ */
