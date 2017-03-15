/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "Robot2017.h"

using namespace ard;

int main( void )
{
    //Spawn ARD instance
    ROBOT.bootOs();

    return 0;
}

extern String getExeVersion()
{
    return String("Version robotA : ") + __DATE__ + " " + __TIME__;
}

