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

//Note that Python yahdlc binding can't go over 8 (header) + 512
#define SERIAL_BUF_SIZE 200
#define HDLC_BUF_SIZE 200

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
    class TeleopThread: public IThread, public ILogChannel
    {
    public:
        TeleopThread() = default;
        virtual ~TeleopThread() = default;

        //Implements IThreads : creates the thread
        void init() override;

        //Implements IThreads : reads the serial inputs
        void run() override;

        //Get any teleop event (non-const on purpose)
        IEvent* getEvent(eTeleopEvtId id);

        //Implements ILogChannel : push a log on the serial link
        virtual void log(LogMsg const & log) override;

        //Implements ILogChannel : returns true if the communication is established
        virtual bool isReady() const override;

    private:
        Event<1> events[EVT_MAX];
        
        //This method is not used, it is just keept as a reminder on how to quickly use a serial com with the Arduino libs.
        //today we prefer the use of google protobuf but they requires a "partner" program to tchat with it, whereas the
        //simple serial interface may be used withy any serial terminal.
        void simpleSerialRun();

        //Decode the message
        void handleMsg(char const * msg, size_t msgLength);

        //reception buffers
        unsigned int serial_index = 0;
        char serial_recv_buffer[SERIAL_BUF_SIZE];
        char hdlc_recv_framebuffer[HDLC_BUF_SIZE];

        //emission buffers. hdlc buffer are flat buffers
        //because yahdlc is not able to read from ring buffers
        char msg_send_buffer[SERIAL_BUF_SIZE];
        char hdlc_send_framebuffer[HDLC_BUF_SIZE];

        //create an hdlc frame from msg buffer and send it byte by bytes
        void writeMsg(unsigned int byteToWrite);

        //read bytes one by one on the serial link and store them in the serial buffer
        //@return bytes read
        unsigned int feedReadBuffer();

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
