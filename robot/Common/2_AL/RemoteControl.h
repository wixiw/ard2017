/*
 * RemoteControlThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_TELEOPTHREAD_H_
#define ROBOTS_TELEOPTHREAD_H_

#include "RSP.h"
#include "proto/RemoteControl.pb.h"

//Note that Python yahdlc binding can't go over 8 (header) + 512
#define TMP_SERIAL_BUF_SIZE 200
#define TMP_HDLC_BUF_SIZE 200

namespace ard
{
    typedef enum
    {
        EVT_CONFIGURE = 0,
        EVT_START_MATCH,
        EVT_MAX
    } eRemoteControlEvtId;

    /**
     * This class is used to rwait and receive commands from debug serial link
     * The aim of those commands are :
     * - simulate an HW event
     * - teleoperate the robot for tests
     */
    class RemoteControl: public Thread, public ILogChannel
    {
    public:
        RemoteControl();
        virtual ~RemoteControl() = default;

        //Implements Thread : reads the serial inputs
        void run() override;

        //Get any teleop event (non-const on purpose)
        IEvent* getEvent(eRemoteControlEvtId id);

        //Implements ILogChannel : returns true if the communication is established
        virtual bool isReady() const override;

        /**------------------------------
         * Send COM API
         --------------------------------*/
        //Implements ILogChannel : push a log on the serial link
        virtual void log(LogMsg const & log) override;
        
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
        char serial_recv_buffer[TMP_SERIAL_BUF_SIZE];
        char hdlc_recv_framebuffer[TMP_HDLC_BUF_SIZE];

        //emission buffers. hdlc buffer are flat buffers
        //because yahdlc is not able to read from ring buffers
        char msg_send_buffer[TMP_SERIAL_BUF_SIZE];
        char hdlc_send_framebuffer[TMP_HDLC_BUF_SIZE];

        //create an hdlc frame from msg buffer and send it byte by bytes
        void writeMsg(unsigned int byteToWrite);

        //read bytes one by one on the serial link and store them in the serial buffer
        //@return bytes read
        unsigned int feedReadBuffer();

        /**------------------------------
         * Receive COM API
         --------------------------------*/
        void getOsStats             (apb_RemoteControlRequest const & request);
        void configureMatch         (apb_RemoteControlRequest const & request);
        void startMatch             (apb_RemoteControlRequest const & request);
        void setPosition            (apb_RemoteControlRequest const & request);
        void requestGoto            (apb_RemoteControlRequest const & request);
        void requestGotoCap         (apb_RemoteControlRequest const & request);

    };

} /* namespace ard */

#endif /* ROBOTS_TELEOPTHREAD_H_ */
