/*
 * ActuatorThread.h
 *
 *  Created on: 8 nov. 2016
 *      Author: wix
 */

#ifndef ROBOTS_ACTUATORTHREAD_H_
#define ROBOTS_ACTUATORTHREAD_H_

#include "ArdOs.h"
#include "Navigation.h"

namespace ard
{

  class ActuatorThread : public IThread
  {
  public:
    ActuatorThread ();
    virtual
    ~ActuatorThread (){};

    //create the thread
    void init();

    //Implements IThreads : ...
    void
    run ();

  private:
    Navigation nav;
  };

} /* namespace ard */

#endif /* ROBOTS_ACTUATORTHREAD_H_ */
