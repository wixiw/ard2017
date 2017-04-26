/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "Tration.h"

using namespace ard;

/**
 * The little 2016 PMI
 */

int main( void )
{   
    g_robot = new Tration();

    //Build and start robot
    g_robot->bootOs();

    return 0;
}

extern String getExeVersion()
{
    return String("Version Tration : ") + __DATE__ + " " + __TIME__;
}

