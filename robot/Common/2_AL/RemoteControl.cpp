/*
 * RemoteControlThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "RemoteControl.h"
#include "Robot2017.h"

//TODO temp :
#include "com/yahdlc.h"

using namespace ard;

#define HANDLE_MSG(msg)                 \
case apb_RemoteControlRequest_##msg##_tag:     \
{                                       \
    LOG_INFO(#msg " request received."); \
    msg(request);                        \
    break;                              \
}

RemoteControl::RemoteControl():
        Thread("RemotCtl", PRIO_REMOTE_CTRL, STACK_REMOTE_CTRL)
{
    INIT_TABLE_TO_ZERO(serial_recv_buffer);
    INIT_TABLE_TO_ZERO(hdlc_recv_framebuffer);
    INIT_TABLE_TO_ZERO(msg_send_buffer);
    INIT_TABLE_TO_ZERO(hdlc_send_framebuffer);
}

IEvent* RemoteControl::getEvent(eRemoteControlEvtId id)
{
    return &events[id];
}

bool RemoteControl::isReady() const
{
    //TODO a better solution would be welcome, for instance for exchanging info with the serial driver
    // or with a dedicated comOpened message in the communication layer (maybe HDLC manage this ?)
    return true;
}

void RemoteControl::writeMsg(unsigned int byteToWrite)
{
    ASSERT_TEXT(byteToWrite <= 200, "Too many bytes in encoded messages, send buffer overshoot.");
    unsigned int hdlcByteToWrite = 0;

    yahdlc_control_t control;
    control.frame = YAHDLC_FRAME_DATA;
    control.seq_no = 0;

    ASSERT_TEXT( 0 == 
            yahdlc_frame_data(&control, msg_send_buffer, byteToWrite, hdlc_send_framebuffer, &hdlcByteToWrite),
            "Failed to encode hdlc frame.");

    ASSERT_TEXT(hdlcByteToWrite <= 200, "Too many bytes in encoded hldc buffer, send buffer overshoot.");
    for(unsigned int i = 0 ; i < hdlcByteToWrite ; i++)
    {
        Serial.write(hdlc_send_framebuffer[i]);
    }
    INIT_TABLE_TO_ZERO(hdlc_send_framebuffer);
}

unsigned int RemoteControl::feedReadBuffer()
{
    while (Serial.available())
    {
        //As the yahdlc layer requires a flat buffer we "un-ring" the driver ring buffer
        serial_recv_buffer[serial_index] = (char) Serial.read();
        serial_index++;
        ASSERT_TEXT(serial_index <= TMP_SERIAL_BUF_SIZE, "Serial framebuffer overshoot");
    }

    return serial_index;
}

void RemoteControl::run()
{
    //use this function to shortcut the google protobuf layer in case you want a simple way of commanding the robot.
    //simpleSerialRun();

    //hldc local vars
    yahdlc_control_t control;

    size_t nbBytesRead = 0;//nb bytes in the receive buffer
    size_t nbHdlcBytes = 0;//nb bytes parsed by yahdlc that can be thrown away
    unsigned int hdlc_length = 0;

    while (2)
    {
        //There are a lot of overflow and performance issues in the following algorithm, but it's a starting point (anyway the Arduino buffer is worse ...)
        nbBytesRead = feedReadBuffer();
        //if no bytes is present wait a bit and read again
        if ( nbBytesRead == 0)
        {
            vTaskDelay(50);
            continue;
        }

        do
        {
            //clean the buffer that will receive the future frame
            hdlc_length = 0;
            INIT_TABLE_TO_ZERO(hdlc_recv_framebuffer);
            INIT_STRUCT_TO_ZERO(control);

            //as a simple solution we filter char by char, for performance improvments we could modify the Arduino UARTClass
            //and add a ReadAll function to get a longer buffer. As long as only non-operational action is
            //done here, there is no need for performance
            int res = yahdlc_get_data(&control, serial_recv_buffer, nbBytesRead, hdlc_recv_framebuffer, &hdlc_length);
            if (0 < res)
            {
                //clean input buffer : unused data at the front and decoded frame are deleted
                nbHdlcBytes = res + 1; //the result is the index of the last read element in the buffer
                if (nbHdlcBytes == TMP_SERIAL_BUF_SIZE)
                {
                    INIT_TABLE_TO_ZERO(serial_recv_buffer);
                    serial_index = 0;
                }
                else
                {
                    //it's possible that some bytes are present after the decoded frame (ex : several received frames)
                    size_t remainingBytesNb = nbBytesRead - nbHdlcBytes;
                    char* startOfRemainingBytes = serial_recv_buffer + nbHdlcBytes; //no overflow as full buffer case is treated before
                    memcpy(serial_recv_buffer, startOfRemainingBytes, remainingBytesNb);
                    //unused bytes are zeroed
                    memset(serial_recv_buffer + remainingBytesNb, 0, nbBytesRead - remainingBytesNb);
                    serial_index = remainingBytesNb;
                }

#ifdef ARD_DEBUG
                hdlc_recv_framebuffer[hdlc_length] = 0;
                hdlc_recv_framebuffer[hdlc_length + 1] = 0;
                //treat the decoded frame
                LOG_DEBUG("HDLC frame received size=" + String(hdlc_length) + " msg=[" + String(hdlc_recv_framebuffer) + "]");
#endif
                handleMsg(hdlc_recv_framebuffer, hdlc_length);
            }
        } while (hdlc_length != 0); //there may be several frames, so as long as a frame is found, unpile buffer

        //clean dust bytes if an incomplete frame is holder the buffer beginning
        yahdlc_state_t serial_buf_state;
        yahdlc_get_state(&serial_buf_state);
        if (serial_buf_state.start_index < 0) //no start found, mean dust bytes at the begining of src buffer
        {
            memset(serial_recv_buffer, 0, nbBytesRead);
            serial_index = 0;
        }

        vTaskDelay(1);
    } //end while(2)
}

void RemoteControl::handleMsg(char const * msg, size_t msgLength)
{
    apb_RemoteControlRequest request = apb_RemoteControlRequest_init_default;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer((pb_byte_t*) msg, msgLength);

    /* Now we are ready to decode the message. */
    if (!pb_decode(&stream, apb_RemoteControlRequest_fields, &request))
    {
        LOG_ERROR("Failed to deserialize message : " + String(PB_GET_ERROR(&stream)));
        return;
    }

    switch (request.which_type)
    {
        HANDLE_MSG(getOsStats)
        HANDLE_MSG(configureMatch)
        HANDLE_MSG(startMatch)
        HANDLE_MSG(setPosition)
        HANDLE_MSG(requestGoto)
        HANDLE_MSG(requestGotoCap)

        default:
        {
            LOG_ERROR("Failed to identify message type : " + String((int)request.which_type));
            break;
        }
    }

}

/**
 * Receive COM API
 */

void RemoteControl::getOsStats(apb_RemoteControlRequest const & request)
{
    char text[40];
    char* pcWriteBuffer = text;

    LOG_INFO("------- ArdOs Stats  -------");
    LOG_INFO("|   Thread   |  Free stack |");
    LOG_INFO("----------------------------");

    TaskStatus_t pxTaskStatusArray[PRIO_MAX];
    UBaseType_t uxArraySize = PRIO_MAX;
    uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, NULL );

    /* Create a human readable table from the binary data. */
    for( UBaseType_t x = 0; x < uxArraySize; x++ )
    {
        /* Write the task name to the string, padding with spaces so it
        can be printed in tabular form more easily. */
        pcWriteBuffer[0] = ' ';
        pcWriteBuffer[1] = ' ';
        pcWriteBuffer = prvWriteNameToBuffer( pcWriteBuffer+2, pxTaskStatusArray[ x ].pcTaskName );

        /* Write the rest of the string. */
        sprintf( pcWriteBuffer, "\t%u", ( unsigned int ) pxTaskStatusArray[ x ].usStackHighWaterMark);
        pcWriteBuffer = text;

        LOG_INFO(String(text));
    }

    LOG_INFO("-----------------------------------------------");
    LOG_INFO(String("  Free heap size : ") + xPortGetFreeHeapSize() + "o");
    LOG_INFO("-----------------------------------------------");

}

void RemoteControl::configureMatch(apb_RemoteControlRequest const & request)
{

    uint8_t strategy = request.type.configureMatch.strategy;
    eColor color = (eColor)(request.type.configureMatch.matchColor);
    ROBOT.strategy.configureMatch(strategy, color);
    events[EVT_CONFIGURE].publish();
}

void RemoteControl::startMatch(apb_RemoteControlRequest const & request)
{
    events[EVT_START_MATCH].publish();
}

void RemoteControl::setPosition(apb_RemoteControlRequest const & request)
{
    ROBOT.nav.setPosition(
            request.type.setPosition.x,
            request.type.setPosition.y,
            request.type.setPosition.h);
}

void RemoteControl::requestGoto(apb_RemoteControlRequest const & request)
{
    ROBOT.nav.goTo(
            request.type.requestGoto.target.x,
            request.type.requestGoto.target.y,
            request.type.requestGoto.direction);
}

void RemoteControl::requestGotoCap(apb_RemoteControlRequest const & request)
{
    ROBOT.nav.goToCap(
            request.type.requestGotoCap.target.x,
            request.type.requestGotoCap.target.y,
            request.type.requestGotoCap.target.h,
            request.type.requestGotoCap.direction);
}

/**
 * Send COM API
 */

void RemoteControl::log(LogMsg const & log)
{
    apb_RemoteControlResponse response = apb_RemoteControlResponse_init_default;

    /* Create a stream that will write to our buffer. */
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t*) msg_send_buffer, sizeof(msg_send_buffer));
    response.which_type = apb_RemoteControlResponse_log_tag;
    response.type.log.date  = log.date;
    response.type.log.level = log.level;
    strcpy(response.type.log.component, log.component.c_str());
    strcpy(response.type.log.text, log.text.c_str());

    /* Now we are ready to encode the message! */
    ASSERT_TEXT(pb_encode(&stream, apb_RemoteControlResponse_fields, &response), "Failed to encode Log message.");
    writeMsg(stream.bytes_written);
}

