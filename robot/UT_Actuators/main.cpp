/*
 * UT_Actuators.cpp
 *
 * Created: 23/11/2016 23:58:55
 * Author : wix
 */ 


#include "Arduino.h"
extern void UT_Steppers(void);
extern void UT_LEDs(void);

int main(void)
{
    UT_LEDs();
    //UT_Steppers();
    
    // should never return
	while(1);
}

extern String getExeVersion()
{
    return String("Version test : ") + __DATE__ + " " + __TIME__;
}
