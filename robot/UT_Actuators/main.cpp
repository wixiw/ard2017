/*
 * UT_Actuators.cpp
 *
 * Created: 23/11/2016 23:58:55
 * Author : wix
 */ 


#include "BSP.hpp"
#include "UT_AccelServo.h"
#include "UT_Stepper.h"
#include "UT_Leds.h"

using namespace ard;

int main(void)
{
    //UT_LEDs();
    //UT_Steppers();
    UT_AccelServo();
    
    // should never return
    ASSERT(false);
	while(1);
}

extern String getExeVersion()
{
    return String("Version test : ") + __DATE__ + " " + __TIME__;
}
