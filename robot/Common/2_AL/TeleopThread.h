/*
 * TeleopThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_TELEOPTHREAD_H_
#define ROBOTS_TELEOPTHREAD_H_

#include "RSP.h"

namespace ard
{
    typedef enum
    {
        EVT_CONFIGURE = 0,
        EVT_START_MATCH,
        EVT_MAX
    } eTeleopEvtId;

    /**
     * This class is used to rwait and receive commands from debug serial link
     * The aim of those commands are :
     * - simulate an HW event
     * - teleoperate the robot for tests
     */
    class TeleopThread: public IThread
    {
    public:
        TeleopThread() = default;
        virtual ~TeleopThread() = default;

        //Implements IThreads :create the thread
        void init() override;

        //Implements IThreads : reads the serial inputs
        void run() override;

        //Get any teleop event
        IEvent* getEvent(eTeleopEvtId id);

    private:
        Event<1> events[EVT_MAX];

    };

} /* namespace ard */

#endif /* ROBOTS_TELEOPTHREAD_H_ */
