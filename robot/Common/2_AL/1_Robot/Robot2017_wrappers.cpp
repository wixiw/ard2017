/*
 * Robot2017_wrappers.cpp
 *
 *  Created on: 1 mai 2017
 *      Author: wix
 */

#include "Robot2017.h"

using namespace ard;

extern "C"
{
    extern void errorBlink(int n);
}

//check for Interrupt context !
//check for C++ demangling problems
void ardAssertImpl(bool condition, char const* file, unsigned int line, char const* text)
{
    if(!condition)
    {
        if( ArdOs::getState() == ArdOs::RUNNING && !Thread::interruptContext() )
        {
            LOG_ASSERT(String(file) +":" + line + String(text));
            if(Robot2017::instance)
                Robot2017::instance->dieMotherFucker();
        }
        else
        {
            errorBlink(3);
        }
    }
}

//Use this interrupt to execute periodic stuff that shall run at a very high frequency (typically steppers)
//Obviously at such a frequence a Thread is too heavy as the context-switch duration would be higher than the period
void veryFast_interrupt()
{
    //  DEBUG_SET_HIGH(); //uncomment to check period and delay with oscilloscope
    Robot2017::instance->nav.updateFromInterrupt();
    //  DEBUG_SET_LOW(); //uncomment to check period and delay with oscilloscope
}

//Use this interrupt to execute period stuff that shall run at a high frequency
//At this frequence a Thread is quite heavy as the context-switch duration would be roughtly equal to the period
void fast_interrupt()
{
//    DEBUG_SET_HIGH(); //uncomment to check period and delay with oscilloscope
    gpioToolsIsrCallback(PERIOD_FAST_IT_US);
//    DEBUG_SET_LOW(); //uncomment to check period and delay with oscilloscope
}

