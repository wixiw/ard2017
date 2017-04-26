/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "Pen.h"

using namespace ard;

/**
 * The new 2017 robot
 */

int main( void )
{   
    //new required to handle the fact that FreeRtos will destroy the default stack
    g_robot = new Pen();

    //Build and start robot
    g_robot->bootOs();

    return 0;
}

extern String getExeVersion()
{
    return String("Version Pen : ") + __DATE__ + " " + __TIME__;
}

