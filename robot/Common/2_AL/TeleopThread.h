/*
 * TeleopThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_TELEOPTHREAD_H_
#define ROBOTS_TELEOPTHREAD_H_

#include "RSP.h"
#include "proto/Teleop.pb.h"

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

        //Implements IThreads : creates the thread
        void init() override;

        //Implements IThreads : reads the serial inputs
        void run() override;

        //Get any teleop event
        IEvent* getEvent(eTeleopEvtId id);

    private:
        Event<1> events[EVT_MAX];
        
        //This method is not used, it is just keept as a reminder on how to quickly use a serial com with the Arduino libs.
        //today we prefer the use of google protobuf but they requires a "partner" program to tchat with it, whereas the
        //simple serial interface may be used withy any serial terminal.
        void simpleSerialRun();

        //Decode the message
        void handleMsg(unsigned char const * msg, size_t msgLength);

        /**
         * Receive COM API
         */
        void getOsStats             (apb_TeleopRequest const & request);
        void configureMatch         (apb_TeleopRequest const & request);
        void startMatch             (apb_TeleopRequest const & request);
        void setPosition            (apb_TeleopRequest const & request);
        void requestGoto            (apb_TeleopRequest const & request);
        void requestGotoCap         (apb_TeleopRequest const & request);

    };

} /* namespace ard */

#endif /* ROBOTS_TELEOPTHREAD_H_ */
