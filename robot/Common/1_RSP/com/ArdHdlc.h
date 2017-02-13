/*
 * ArdHdlc.h
 *
 *  Created on: 10 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_COM_ARDHDLC_H_
#define ROBOT_COMMON_1_RSP_COM_ARDHDLC_H_

#include "ComInterfaces.h"

//Note that Python yahdlc binding can't go over 8 (header, taking addr and ctrl field escape into account) + 512
#define MSG_SIZE 512
#define HDLC_HEADER 8
#define HDLC_BUF_SIZE (MSG_SIZE+HDLC_HEADER)

namespace ard
{

    class ArdHdlc: public ICom
    {
    public:
        ArdHdlc(String name, HardwareSerial& serialDriver);
        virtual ~ArdHdlc() = default;

        //Implements ICom interface.
        virtual void setListener(IComListener* newListener) override;

        //Implements ICom interface : open the serial link with config set by configure() method
        virtual bool connect() override;

        //Implements ICom interface : close the serial link
        virtual void disconnect() override;

        //Implements ICom interface : check the serial link
        virtual bool isConnected() const override;

        //Implements ICom interface : create an hdlc frame from msg buffer and send it byte by bytes
        virtual bool sendMsg(char const * msg, size_t msgLength) override;

        //read bytes one by one on the serial link and store them in the serial buffer
        //@return true is new bytes has been read
        bool feedReadBuffer();

        //parse input buffer and search for an hdlc frame
        void parseBuffer();

    private:
        String name;
        IComListener* listener;

        //The serial link on which ArdHdlc is mapped
        HardwareSerial& physicalLink;

        //reception buffers
        unsigned int serial_index = 0;
        char serial_recv_buffer[HDLC_BUF_SIZE];
        char hdlc_recv_framebuffer[HDLC_BUF_SIZE];

        //emission buffers. hdlc buffer are flat buffers
        //because yahdlc is not able to read from ring buffers
        size_t nbParsedBytes = 0;//nb bytes parsed by yahdlc that can be thrown away
        size_t hdlc_length = 0;
        char hdlc_send_framebuffer[HDLC_BUF_SIZE];
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_COM_ARDHDLC_H_ */
