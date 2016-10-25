/*
 * Robot2017.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ROBOT2017_H_
#define ROBOTS_ROBOT2017_H_

#include "BSP.h"
#include "RSP.h"
#include "StrategyThread.h"

namespace ard
{

  /**
   * This class assemble all the robot SW elements into a system
   * It aims at being the unique global var.
   *
   * Most members are public as this class aims at gathering all object references
   * It would be a pain (and stupid) to catch all them with non-const getter
   */
  class Robot2017
  {
  public:
    //Assemble all object instances
    Robot2017();

    //Initialize instances and start the robot
    //This function never ends
    void boot();

    //Freeze the robot so we are sure it doesn't do
    //anything until the end of the match
    //Note : it will still do the funny action
    void dieMotherFucher();

    //Threads
    LogThread& 			log;
    TeleopThread 		teleop;
    StrategyThread		strategy;

    //Hardware peripherals
    HmiThread			hmi;
    AccelStepper		stepperG;
    AccelStepper		stepperD;

  };

} /* namespace ard */

#endif /* ROBOTS_ROBOT2017_H_ */
