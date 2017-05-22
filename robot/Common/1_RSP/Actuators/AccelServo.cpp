/*
 * AccelServo.cpp
 *
 *  Created on: 27 avr. 2017
 *      Author: wix
 */

#include "AccelServo.h"
using namespace ard;

AccelServo::AccelServo(String const& name, int servoOutPin, uint16_t _min, uint16_t _max):
        PolledObject(name),
        servo(servoOutPin),
        cmdVmaxInc(150),
        cmdAccMaxInc(30),
        startDeccDelta(0),
        target(servo.readMicroseconds()),
        cmd(servo.readMicroseconds()),
        cmdSpeed(0),
        direction(0),
        mutex(),
        enabled(false),
        m_min(_min),
        m_max(_max)
{
    ASSERT_TEXT(PERIOD_ACTUATORS==20,"AccelServo acc/vmax configs depends on period, if you change the period, check the configs");
}

void AccelServo::update(DelayMs sinceLastCall)
{
    //Servo is not powered, no need to command
    if(!enabled)
        return;

    //target is already reached, nothing to do
    if(cmd == target)
        return;

    mutex.lock();

    DelayUs delta = abs(int32_t(target) - int32_t(cmd));

    if( cmdAccMaxInc == 0 )
    {
        cmdSpeed = cmdVmaxInc;
        if(delta < cmdSpeed)
            cmdSpeed = delta;
    }
    else
    {
        //If the distance to target is little, start deccelerating
        if( delta <= startDeccDelta)
        {
            //If a decceleration is configured and last steps are not reached
            if( cmdAccMaxInc < cmdSpeed)
                cmdSpeed -= cmdAccMaxInc;
            //Else : last step reach
            else
            {               
                //Last step requires a move at low speed
                cmdSpeed = cmdAccMaxInc;
            }
        }
        //else accelerate or stay at max speed
        else
        {
            //If acceleration is configured
            if( cmdSpeed < cmdVmaxInc )
            {
                //degenerated case for little orders
                if( delta/2 < cmdAccMaxInc)
                    cmdSpeed = delta/2;
                else
                    cmdSpeed += cmdAccMaxInc;

                //stay at vmax
                if( cmdVmaxInc < cmdSpeed)
                    cmdSpeed = cmdVmaxInc;
            }
        }
    }
    
    //ensure no overshoot is present
    //Typical case : a bi-ramp move may be irregular so last step may brake less than expected
    //Typical case : a new command is send on a moving servo, which a close target at which we can't brake.
    if( delta < cmdSpeed)
        cmdSpeed = delta;

    //ensure a minimal move is required
    ASSERT_TEXT(cmdSpeed != 0, "Speed is null");

    //increment cmd to reach target
    cmd += direction * cmdSpeed;
    servo.writeMicroseconds(cmd);

    mutex.unlock();
}

void AccelServo::disable()
{
    mutex.lock();
    enabled = false;
    servo.disable();
    mutex.unlock();
}

void AccelServo::setVmax(DelayMs increment)
{
    ASSERT(increment != 0);
    cmd = target;
    cmdSpeed = 0;
    cmdVmaxInc = increment;
}

void AccelServo::setAccMax(DelayMs increment)
{
    cmd = target;
    cmdSpeed = 0;
    cmdAccMaxInc = increment;
}

void AccelServo::goTo(uint16_t _target)
{
    if (_target < m_min)
        _target = m_min;
    else if (_target > m_max)
        _target = m_max;

    mutex.lock();
    target = servo.perThousandToUs(_target);

    //If servo is not already enabled, enabled it and go to target directly
    if(!enabled)
    {
        enabled = true;
        ASSERT(servo.enable());
        cmd = target;
        servo.writeMicroseconds(cmd);
    }
    else
    {
        //Get the time at which we need to stop moving
        computeStartDecc();

        if( cmd <= target )
            direction = 1;
        else
            direction = -1;
    }

    mutex.unlock();
}

void AccelServo::computeStartDecc()
{
    if(cmdAccMaxInc)
    {
        //Nominal case : deccelerate from vmax
        uint16_t nPeriods = max(cmdVmaxInc/cmdAccMaxInc, 1);
        startDeccDelta = cmdAccMaxInc*nPeriods*(nPeriods+1)/2;

        //Degenerated case : "triangle" motion profile with acc/decc (vmax is never reached)
        DelayUs delta = abs(int32_t(target) - int32_t(cmd));
        if( delta < 2*startDeccDelta)
        {
            startDeccDelta = delta/2;
			if( startDeccDelta == 0 )
				startDeccDelta = 1;
        }
    }
    else
    {
        startDeccDelta = 0;
    }
}
