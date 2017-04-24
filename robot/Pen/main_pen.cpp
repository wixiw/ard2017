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
    Pen& robotPen = Pen::getInstance();

    //Build and start robot
    robotPen.bootOs();

    return 0;
}

extern String getExeVersion()
{
    return String("Version Pen : ") + __DATE__ + " " + __TIME__;
}

