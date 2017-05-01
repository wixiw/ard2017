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
    RemoteControl* remoteControl = new RemoteControl(*robot);
    ASSERT(remoteControl);

    //Build and start robot
    robot->init(remoteControl);
    remoteControl->start();
    robot->boot();

    return 0;
}

