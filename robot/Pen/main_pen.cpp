/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "Pen.h"
#include "AL.h"

using namespace ard;

/**
 * The new 2017 robot
 */

int main( void )
{   
    //a call to new() is required to handle the fact that FreeRtos will destroy the default stack
    Pen* robot = new Pen();
    ASSERT(robot);
    RemoteControl* remoteControl = new RemoteControl(*robot);
    ASSERT(remoteControl);

    //Build and start robot
    robot->init(remoteControl);
    remoteControl->start();
    robot->boot();
    
    return 0;
}


