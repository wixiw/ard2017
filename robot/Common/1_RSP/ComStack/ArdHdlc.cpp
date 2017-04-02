/*
 * ArdHdlc.cpp
 *
 *  Created on: 10 janv. 2017
 *      Author: wix
 */

#include "ArdHdlc.h"
#include "Log.h"

using namespace ard;

ArdHdlc::ArdHdlc(String name, ISerialDriver& serialDriver):
        listener(NULL),
        physicalLink(serialDriver),
        bytesInRecvBuf(0),
        dropMsgCount(0),
        nbParsedBytes(0),
        hdlc_length(0)
{
    INIT_TABLE_TO_ZERO(serial_recv_buffer);
    INIT_TABLE_TO_ZERO(hdlc_recv_framebuffer);
    INIT_TABLE_TO_ZERO(hdlc_send_framebuffer);
    ardHdlc_resetState(&hdlcState);
}

void ArdHdlc::setListener(IComListener* newListener)
{
    ASSERT_TEXT(listener == NULL, "Communication listener has already been set");
    ASSERT_TEXT(newListener != NULL, "Communication : new listener is NULL");
    listener = newListener;
}

bool ArdHdlc::connect()
{
    if( listener )
        listener->com_connected(this);
    return true;
}

void ArdHdlc::disconnect()
{
    if( listener )
        listener->com_disconnected(this);
}

void ArdHdlc::readAll()
{
    bool newBytes = false;
    uint8_t byte = 0;
    
    do 
    {
        physicalLink.read(&byte);
        
        //Check that there is room in the receive buffer to get the next char
        if(SERIAL_BUF_SIZE < bytesInRecvBuf)
        {
            LOG_ERROR("Serial receive framebuffer overshoot");
            break;
        }

        //If overflow or Hw error occured in the driver, throw everything out
        if(physicalLink.getError() != ISerialDriver::ERR_NO_ERROR)
        {
            ardHdlc_resetState(&hdlcState);
            memset(serial_recv_buffer, 0, SERIAL_BUF_SIZE);
            bytesInRecvBuf = 0;
            INIT_TABLE_TO_ZERO(hdlc_recv_framebuffer);
            dropMsgCount++;
            LOG_ERROR("Serial HW error : one or more bytes lost, buffer reseted.");
        }
        else
        {
            //TODO debug : check that no data are overwritten
            ASSERT(serial_recv_buffer[bytesInRecvBuf] == 0);

            //As the yahdlc layer requires a flat buffer we "un-ring" the driver ring buffer
            serial_recv_buffer[bytesInRecvBuf] = byte;
            bytesInRecvBuf++;
            newBytes = true;
        }
    } while (physicalLink.getNbBytesReceived());

    if(newBytes)
    {
        parseBuffer();
    }
}


void ArdHdlc::parseBuffer()
{
    bool msgFound = false;
    unsigned int lastBytesInRecvBuf = 0;
    
    while (lastBytesInRecvBuf != bytesInRecvBuf) //there may be several frames, so as long as we find some work to do, unpile the buffer
    {
        //hldc local vars
        ardHdlc_control_t control;
        lastBytesInRecvBuf = bytesInRecvBuf;

        //clean the buffer that will receive the future frame
        hdlc_length = 0;
        INIT_STRUCT_TO_ZERO(control);

        //as a simple solution we filter char by char, for performance improvments we could modify the Arduino UARTClass
        //and add a ReadAll function to get a longer buffer. As long as only non-operational action is
        //done here, there is no need for performance
        int res = ardHdlc_decodeFrame(&hdlcState, &control, serial_recv_buffer, bytesInRecvBuf, hdlc_recv_framebuffer, &hdlc_length);
        if (0 < res)
        {
            //A clean message has been parsed : send it to the listener and clean the input buffer
#ifdef ARD_DEBUG
            //checksum set to zero so that it's not displayed
            hdlc_recv_framebuffer[hdlc_length] = 0;
            hdlc_recv_framebuffer[hdlc_length + 1] = 0;
            //LOG_DEBUG("HDLC frame received size=" + String(hdlc_length));
#endif
            //A message has been found
            msgFound = true;
            if(listener)
                listener->handleMsg(this, hdlc_recv_framebuffer, hdlc_length);
            
            //clean output buffer
            INIT_TABLE_TO_ZERO(hdlc_recv_framebuffer);

            nbParsedBytes = res + 1; //the result is the index of the last read element in the buffer
            
            //clean input buffer : all data are read and copied into hdlc_recv_framebuffer
            if (nbParsedBytes == SERIAL_BUF_SIZE)
            {
                INIT_TABLE_TO_ZERO(serial_recv_buffer);
                bytesInRecvBuf = 0;
                lastBytesInRecvBuf = 0;
            }
            else
            {
                //it's possible that some bytes are present after the decoded frame (ex : several received frames)
                size_t remainingBytesNb = bytesInRecvBuf - nbParsedBytes;

                //remaining bytes are moved to the top of the buffer
                throwUnusedBytes(nbParsedBytes, remainingBytesNb);
                bytesInRecvBuf = remainingBytesNb;
            }
        }
        else
        {
            //EINVAL should never happen as it's a dev error
            ASSERT(res != -ARDHDLC_EINVAL);

            //EFCS : message is checksum malformed, to be discarded
            //ETOOSHORT : message is too short, to be discarded
            if( res == -ARDHDLC_EFCS || res == -ARDHDLC_ETOOSHORT )
            {
                size_t startOfRemainingData = hdlc_length;//TODO c'est louche, c'est peut être nbParsedBytes
                ASSERT(startOfRemainingData <= bytesInRecvBuf);
                size_t remainingBytesNb = bytesInRecvBuf - startOfRemainingData;
                throwUnusedBytes(startOfRemainingData, remainingBytesNb);
                bytesInRecvBuf = remainingBytesNb;
                if( res == -ARDHDLC_EFCS )
                    LOG_ERROR(String("Msg CRC error in received msg, data discarded : ") + hdlc_recv_framebuffer);
                else
                    LOG_ERROR(String("Msg too short in received msg, data discarded : ") + hdlc_recv_framebuffer);
                    
                //clean output buffer
                INIT_TABLE_TO_ZERO(hdlc_recv_framebuffer);
            }
            
            //ENOMSG is not an error, it just mean the message is not received completely
        }
    } 

    //If no message has been found, we test if we are in the progress of parsing a message
    //If no start found, we are not processing a message and the buffer is full of shit to throw out
    if(!msgFound && hdlcState.start_index < 0)
    {
        ardHdlc_resetState(&hdlcState);
        memset(serial_recv_buffer, 0, bytesInRecvBuf);
        bytesInRecvBuf = 0;
        //clean output buffer
        INIT_TABLE_TO_ZERO(hdlc_recv_framebuffer);
    }        
}

bool ArdHdlc::sendMsg(char const * msg, size_t msgLength)
{
    ardHdlc_control_t control;
    control.frame = ARDHDLC_FRAME_DATA;
    control.seq_no = 0;

    ASSERT_TEXT(msgLength <= SERIAL_BUF_SIZE, "Too many bytes in encoded messages, send buffer overshoot.");

    unsigned int hdlcByteToWrite = SERIAL_BUF_SIZE;
    int res = ardHdlc_createDataFrame(&control, msg, msgLength, hdlc_send_framebuffer, &hdlcByteToWrite);
    for(unsigned int i = 0 ; i < hdlcByteToWrite ; i++)
    {
        physicalLink.write(hdlc_send_framebuffer[i]);
    }
    INIT_TABLE_TO_ZERO(hdlc_send_framebuffer);
    
    return res == ARDHDLC_ESUCCESS;
}

bool ArdHdlc::isConnected() const
{
	//TODO
	return true;
}

void ArdHdlc::throwUnusedBytes(size_t startIndex, size_t remainingBytesNb)
{       
    //move usefull data to the beginning
    memcpy(serial_recv_buffer, serial_recv_buffer+startIndex, remainingBytesNb);

    //if the copied bytes doesn't overlap all the thrown out area : fill with zero
    if( remainingBytesNb < startIndex )
    {
        memset(serial_recv_buffer+remainingBytesNb, 0, startIndex - remainingBytesNb);
    }
    //fill the old msg position with zeroes
    memset(serial_recv_buffer+startIndex, 0, remainingBytesNb);
    
    for(int i = remainingBytesNb ; i < SERIAL_BUF_SIZE ; i++)
        ASSERT(serial_recv_buffer[i] == 0);
}
