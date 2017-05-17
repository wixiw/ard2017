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
    //new required to handle the fact that FreeRtos will destroy the default stack
    Tration* robot = new Tration();
    ASSERT(robot);
#ifdef BUILD_MOTION_CONTROL
    RemoteControl* remoteControl = new RemoteControl(*robot);
    ASSERT(remoteControl);
#endif

    //Build and start robot
#ifdef BUILD_MOTION_CONTROL
    robot->init(remoteControl);
    remoteControl->start();
#else
    robot->init(NULL);
#endif
    robot->boot();

    return 0;
}

