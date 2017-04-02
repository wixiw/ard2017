#include "Arduino.h"
#include "FreeRTOS_ARM.h"
#include "BSP.hpp"

//extern void context_switch_duration();
//extern void ard_signal();
//extern void ard_semaphore();
extern void interrupt_mask();

BSP bsp;

//------------------------------------------------------------------------------
int main()
{
    //bsp.serial0.start(/*baurate = */115200);
        
    //Select the test you want
    //context_switch_duration();
    //ard_signal();
    //ard_semaphore();
    interrupt_mask();

	// should never return
	while(1);
}

extern String getExeVersion()
{
    return String("Version test : ") + __DATE__ + " " + __TIME__;
}
