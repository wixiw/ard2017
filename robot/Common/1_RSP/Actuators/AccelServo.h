/*
 * AccelServo.h
 *
 *  Created on: 27 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_ACTUATORS_ACCELSERVO_H_
#define ROBOT_COMMON_1_RSP_ACTUATORS_ACCELSERVO_H_

#include "ArdOs.h"
#include "Servo.h.old"

namespace ard
{
    /**
     * AccelServo is a decorator of the Servo class we can use to introduce
     * a trapezoidal motion profile
     */
    class AccelServo: public PolledObject, public IServo
    {
    public:
        /**
         * Attach min and max pins
         */
        AccelServo(String const& name,
        		int servoOutPin,
				uint16_t _min = 0U,
				uint16_t _max = 1000U,
				bool inverted = false);

        //Implements PolledObject
        void update(DelayMs sinceLastCall) override;

        //Implements IServo
        void disable() override;

        //Implements IServo
        uint16_t read() override;

        /**
         * The length of a PWM increment during a vmax period, typically 150us for standard servos
         * Do not call during an order, it's not thread safe
         */
        void setVmax(DelayUs increment /* us */);

        /**
         * The length of a PWM increment during an acceleration period, 30us is a reasonable value
         * or 0 to disable
         * Do not call during an order, it's not thread safe
         */
        void setAccMax(DelayUs increment /* us*/);

        /**
         * Implements IServo
         *
         * Goto Target position : 0 min, 1000 max
         * enable the servo power if not already enabled.
         * There is no position control when enabling
         * as we cannot know where the servo is.
         *
         * Cmd range :
         * 0 means : go until min switch is reached
         * 1000 means : go until max switch is reached
         */
        virtual void goTo(uint16_t _target) override;

        /**
         * Motion command finished
         */
        virtual bool isTargetReached(){return cmd == target;};

    protected:
        Servo servo;
        DelayUs cmdVmaxInc; //The duration of the PWM high value to add to previous value duration vmax situation
        DelayUs cmdAccMaxInc; //The duration of the PWM high value to add to previous value
        DelayUs startDeccDelta; //The PWM duration at which we should begin to slow
        DelayUs target; //the target set with goTo
        DelayUs cmd; //the current position command evolving to reach target
        DelayUs cmdSpeed; //the current command speed making a trapeze to reach target
        int8_t  direction; //1 or -1 depending on move direction (it's an interval info, not a servo CC/CCW reverse config)
        Mutex mutex;
        bool enabled;
        uint16_t m_min;
        uint16_t m_max;
        bool inverted;

        void computeStartDecc();
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_ACTUATORS_ACCELSERVO_H_ */
