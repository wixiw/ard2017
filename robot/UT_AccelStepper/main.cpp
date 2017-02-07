/*
 * UT_AccelStepper.cpp
 *
 * Created: 23/11/2016 23:58:55
 * Author : wix
 */ 


#include "sam.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();

    /* Replace with your application code */
    while (1) 
    {
    }
}

extern String getExeVersion()
{
    return String("Version robotA : ") + __DATE__ + " " + __TIME__;
}
