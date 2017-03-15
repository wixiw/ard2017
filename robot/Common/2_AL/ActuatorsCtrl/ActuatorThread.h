/*
 * ActuatorThread.h
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ACTUATORTHREAD_H_
#define ROBOTS_ACTUATORTHREAD_H_

#include "RSP.h"
#include "ActuatorX.h"

namespace ard
{
  class ActuatorThread : public PollerThread
  {
  public:
    ActuatorThread();

    //Overrides Thread : register polled objects
    void init() override;

    //used for telemetry or any instropection
    apb_ActuatorsState getState();

    //data are public to prevent having to write a decorator, but anyone is welcome to do it
    ColorSensor stockColor;
    ActuatorX   claws;
  };

} /* namespace ard */

#endif /* ROBOTS_ACTUATORTHREAD_H_ */
