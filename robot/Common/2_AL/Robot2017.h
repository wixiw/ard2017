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
#include "Navigation.h"
#include "ActuatorThread.h"
#include "StrategyThread.h"
#include "ActuatorX.h"

#define ROBOT Robot2017::getInstance()

namespace ard
{

  /**
   * This class assemble all the robot SW elements into a system
   * It aims at being the unique global var so that var creation order is managed
   * by this class constructor.
   *
   * Most members are public as this class aims at gathering all object references
   * It would be a pain (and stupid) to catch all them with non-const getter
   */
  class Robot2017
  {
  public:
    //retrieve the singleton instance (you should prefer the use of the g_ArdOs maccro)
    static Robot2017&
    getInstance ()
    {
      return instance;
    }
    ;

    //Initialize instances and start the robot
    //This function never ends
    void boot();

    //Freeze the robot so we are sure it doesn't do
    //anything until the end of the match
    //Note : it will still do the funny action
    void dieMotherFucher();

    //Threads
    HmiThread			hmi;
    LogThread& 			log;
    TeleopThread 		teleop;
    ActuatorThread		actuators;
    StrategyThread		strategy;

    //Components
    Navigation          nav;
	ActuatorX			claws;

  private:
    //singleton instance
    static Robot2017 instance;

    //Assemble all object instances
    //private constructor as its a singleton class
    Robot2017 ();

  };

} /* namespace ard */

#endif /* ROBOTS_ROBOT2017_H_ */
