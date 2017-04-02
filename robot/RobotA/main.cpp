/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "Robot2017.h"

using namespace ard;

int main( void )
{   
    Robot2017& robotA = Robot2017::getInstance();

    //Retrieve and modify config
    apb_Configuration config = robotA.getConfig();
    config.matchDuration = 666;
    robotA.setConfig(config);

    //Build and start robot
    robotA.bootOs();

    return 0;
}

extern String getExeVersion()
{
    return String("Version robotA : ") + __DATE__ + " " + __TIME__;
}

