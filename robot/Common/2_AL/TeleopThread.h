/*
 * TeleopThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_TELEOPTHREAD_H_
#define ROBOTS_TELEOPTHREAD_H_

#include "ArdOs.h"

namespace ard
{
  /**
   * This class is used to rwait and receive commands from debug serial link
   * The aim of those commands are :
   * - simulate an HW event
   * - teleoperate the robot for tests
   */

  class TeleopThread : public IThread
  {
  public:
    TeleopThread ();
    virtual
    ~TeleopThread ()
    {
    }
    ;

    //Implements IThreads :create the thread
    void init() override;

    //Implements IThreads : reads the serial inputs
    void
    run () override;
  };

} /* namespace ard */

#endif /* ROBOTS_TELEOPTHREAD_H_ */
