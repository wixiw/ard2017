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
    class TeleopThread: public IThread, public Hdlc
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

        //Implements Hdlc: send byte on serial link
        void sendByte(uint8_t data) override;

        //Implements Hdlc: decode received frame
        void handleFrame(const uint8_t *framebuffer, uint16_t framelength) override;

        //This method is not used, it is just keept as a reminder on how to quickly use a serial com with the Arduino libs.
        //today we prefer the use of google protobuf but they requires a "partner" program to tchat with it, whereas the
        //simple serial interface may be used withy any serial terminal.
        void simpleSerialRun();

    };

} /* namespace ard */

#endif /* ROBOTS_TELEOPTHREAD_H_ */
