/*
 * ArdHdlc.h
 *
 *  Created on: 10 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_COM_ARDHDLC_H_
#define ROBOT_COMMON_1_RSP_COM_ARDHDLC_H_

#include "ComInterfaces.h"
#include "string/WString.h"
#include <hdlc.h>

//Serial buffer set to 3 times the HDLC value in hdlc.h
#define SERIAL_BUF_SIZE 3*1024

namespace ard
{

    class ArdHdlc: public ICom
    {
    public:
        ArdHdlc(String name, ISerialDriver& serialDriver);
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

        //Implements ICom interface : read all received bytes and call listener when payload is detected
        virtual void readAll() override;

        // for the stats
        uint32_t nbMsgRecv;         // total number of msg received
        uint32_t nbMsgSent;         // total number of msg sent
        uint32_t maxRxBufferLoad;   // max raw data in serial_recv_buffer
        uint32_t maxRxRawMsg;       // max size of single hdlc raw msg in serial_recv_buffer
        uint32_t maxRxPayloadSize;  // max size of hdlc payload
        uint32_t maxTxRawMsg;       // max size of hdlc msg (header + payload + crc + ...)
        //statistic to count dropped messages (in fact it's the number of time we reseted the buffer, it may be more than that, but who care ?)
        uint32_t dropMsgCount;

    private:
        IComListener* listener;

        //The serial link on which ArdHdlc is mapped
        ISerialDriver& physicalLink;

        //reception buffers
        unsigned int bytesInRecvBuf = 0; //usefull bytes in serial_recv_buffer
        char serial_recv_buffer[SERIAL_BUF_SIZE];
        char hdlc_recv_framebuffer[SERIAL_BUF_SIZE];
        
        //hdlc buffer tracking state
        ardHdlc_state_t hdlcState;

        //emission buffers. hdlc buffer are flat buffers
        //because yahdlc is not able to read from ring buffers
        size_t nbParsedBytes = 0;//nb bytes parsed by yahdlc that can be thrown away (in the input serial buffer)
        size_t hdlc_length = 0; //nb bytes in the output buffer after hdlc has parsed the data.
        char hdlc_send_framebuffer[SERIAL_BUF_SIZE];

        //translate bytes from serial_recv_buffer to the left and replace old data with 0
        //Usefull data start from serial_recv_buffer[indexOfUsefullData] and goes up to serial_recv_buffer[indexOfUsefullData+sizeOfUsefullData-1]
        void throwUnusedBytes(size_t indexOfUsefullData, size_t sizeOfUsefullData);

        //parse input buffer and search for an hdlc frame
        void parseBuffer();
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_COM_ARDHDLC_H_ */
