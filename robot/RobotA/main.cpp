/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "Robot2017.h"

using namespace ard;

#include <FreeRTOS_ARM.h>
TaskHandle_t xIdleHandle = NULL;

//------------------------------------------------------------------------------
void setup()
{
    ROBOT.boot();
}

//------------------------------------------------------------------------------
void loop()
{
    static bool once = false;
    if (!once)
    {
        once = true;
        LOG_INFO("Version libArd : " + ROBOT.getVersion());
        LOG_INFO(String("Version robotA : ") + __DATE__ + " " + __TIME__ );
    }

    // idle loop has a very small, configMINIMAL_STACK_SIZE, stack
    // loop must never block
    while (2);
}
