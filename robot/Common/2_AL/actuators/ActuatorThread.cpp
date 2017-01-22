/*
 * ActuatorThread.cpp
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#include "ActuatorThread.h"

using namespace ard;

ActuatorThread::ActuatorThread():
        PollerThread("Actuator", PRIO_ACTUATORS, STACK_ACTUATORS, PERIOD_ACTUATORS, 2)
{
}

void ActuatorThread::init()
{
    addPolledObject(stockColor);
    addPolledObject(claws);

    PollerThread::init();
}

apb_ActuatorsState ActuatorThread::getState()
{
    apb_ActuatorsState state = apb_ActuatorsState_init_default;
    //TODO
    return state;
}




