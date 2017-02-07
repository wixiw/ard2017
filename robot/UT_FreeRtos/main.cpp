#include "Arduino.h"
#include "FreeRTOS_ARM.h"

//extern void context_switch_duration();
extern void ard_signal();

//------------------------------------------------------------------------------
int main()
{
    //Select the test you want
    //context_switch_duration();
    ard_signal();

	// should never return
	while(1);
}

extern String getExeVersion()
{
    return String("Version robotA : ") + __DATE__ + " " + __TIME__;
}
