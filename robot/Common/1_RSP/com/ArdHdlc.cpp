/*
 * ArdHdlc.cpp
 *
 *  Created on: 10 janv. 2017
 *      Author: wix
 */

#include "ArdHdlc.h"
#include "yahdlc.h"

using namespace ard;

ArdHdlc::ArdHdlc(String name, HardwareSerial& serialDriver):
        name(name),
        listener(NULL),
        physicalLink(serialDriver)
{
    INIT_TABLE_TO_ZERO(serial_recv_buffer);
    INIT_TABLE_TO_ZERO(hdlc_recv_framebuffer);
    INIT_TABLE_TO_ZERO(hdlc_send_framebuffer);
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

bool ArdHdlc::feedReadBuffer()
{
    bool newBytes = false;

    while (physicalLink.available())
    {
        //As the yahdlc layer requires a flat buffer we "un-ring" the driver ring buffer
        serial_recv_buffer[serial_index] = (char) physicalLink.read();
        serial_index++;
        newBytes = true;
        ASSERT_TEXT(serial_index <= SERIAL_BUF_SIZE, "Serial framebuffer overshoot");
    }

    return newBytes;
}


void ArdHdlc::parseBuffer()
{
    do
    {
        //hldc local vars
        yahdlc_control_t control;

        //clean the buffer that will receive the future frame
        hdlc_length = 0;
        INIT_TABLE_TO_ZERO(hdlc_recv_framebuffer);
        INIT_STRUCT_TO_ZERO(control);

        //as a simple solution we filter char by char, for performance improvments we could modify the Arduino UARTClass
        //and add a ReadAll function to get a longer buffer. As long as only non-operational action is
        //done here, there is no need for performance
        int res = yahdlc_get_data(&control, serial_recv_buffer, serial_index, hdlc_recv_framebuffer, &hdlc_length);
        if (0 < res)
        {
            //clean input buffer : unused data at the front and decoded frame are deleted
            nbParsedBytes = res + 1; //the result is the index of the last read element in the buffer
            if (nbParsedBytes == SERIAL_BUF_SIZE)
            {
                INIT_TABLE_TO_ZERO(serial_recv_buffer);
                serial_index = 0;
            }
            else
            {
                //it's possible that some bytes are present after the decoded frame (ex : several received frames)
                size_t remainingBytesNb = serial_index - nbParsedBytes;
                char* startOfRemainingBytes = serial_recv_buffer + nbParsedBytes; //no overflow as full buffer case is treated before
                memcpy(serial_recv_buffer, startOfRemainingBytes, remainingBytesNb);
                //unused bytes are zeroed
                memset(serial_recv_buffer + remainingBytesNb, 0, serial_index - remainingBytesNb);
                serial_index = remainingBytesNb;
            }

#ifdef ARD_DEBUG
            //checksum set to zero so that it's not displayed
            hdlc_recv_framebuffer[hdlc_length] = 0;
            hdlc_recv_framebuffer[hdlc_length + 1] = 0;
            //treat the decoded frame
            LOG_DEBUG("HDLC frame received size=" + String(hdlc_length) + " msg=[" + String(hdlc_recv_framebuffer) + "]");
#endif
            if(listener)
                listener->handleMsg(this, hdlc_recv_framebuffer, hdlc_length);
        }
    } while (hdlc_length != 0); //there may be several frames, so as long as a frame is found, unpile buffer

    //clean dust bytes if an incomplete frame is holder the buffer beginning
    yahdlc_state_t serial_buf_state;
    yahdlc_get_state(&serial_buf_state);
    if (serial_buf_state.start_index < 0) //no start found, mean dust bytes at the begining of src buffer
    {
        memset(serial_recv_buffer, 0, serial_index);
        serial_index = 0;
    }
}

bool ArdHdlc::sendMsg(char const * msg, size_t msgLength)
{
    yahdlc_control_t control;
    control.frame = YAHDLC_FRAME_DATA;
    control.seq_no = 0;

    ASSERT_TEXT(msgLength <= MSG_SIZE, "Too many bytes in encoded messages, send buffer overshoot.");

    unsigned int hdlcByteToWrite = 0;
    int res = yahdlc_frame_data(&control, msg, msgLength, hdlc_send_framebuffer, &hdlcByteToWrite);
    ASSERT_TEXT(hdlcByteToWrite <= HDLC_BUF_SIZE, "Too many bytes in encoded hldc buffer, send buffer overshoot.");
    for(unsigned int i = 0 ; i < hdlcByteToWrite ; i++)
    {
        physicalLink.write(hdlc_send_framebuffer[i]);
    }
    INIT_TABLE_TO_ZERO(hdlc_send_framebuffer);
    
    return res == ESUCCESS;
}

bool ard::ArdHdlc::isConnected() const
{
	//TODO
	return true;
}

