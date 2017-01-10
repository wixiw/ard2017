/*
 * TeleopThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "TeleopThread.h"
#include "Robot2017.h"

using namespace ard;

#define HANDLE_MSG(msg)                 \
case apb_TeleopRequest_##msg##_tag:     \
{                                       \
    LOG_INFO(#msg " request received."); \
    msg(request);                        \
    break;                              \
}

void TeleopThread::init()
{
    //create the thread
    g_ArdOs.createThread_Cpp("Teleop", *this, STACK_LOG, PRIO_LOG);

    INIT_TABLE_TO_ZERO(serial_recv_buffer);
    INIT_TABLE_TO_ZERO(hdlc_recv_framebuffer);
    INIT_TABLE_TO_ZERO(msg_send_buffer);
    INIT_TABLE_TO_ZERO(hdlc_send_framebuffer);
}

IEvent* TeleopThread::getEvent(eTeleopEvtId id)
{
    return &events[id];
}

bool TeleopThread::isReady() const
{
    //TODO a better solution would be welcome, for instance for exchanging info with the serial driver
    // or with a dedicated comOpened message in the communication layer (maybe HDLC manage this ?)
    return true;
}

void TeleopThread::writeMsg(unsigned int byteToWrite)
{
    ardAssert(byteToWrite <= 200, "Too many bytes in encoded messages, send buffer overshoot.");
    unsigned int hdlcByteToWrite = 0;

    yahdlc_control_t control;
    control.frame = YAHDLC_FRAME_DATA;
    control.seq_no = 0;

    ardAssert( 0 == 
            yahdlc_frame_data(&control, msg_send_buffer, byteToWrite, hdlc_send_framebuffer, &hdlcByteToWrite),
            "Failed to encode hdlc frame.");

    ardAssert(hdlcByteToWrite <= 200, "Too many bytes in encoded hldc buffer, send buffer overshoot.");
    for(unsigned int i = 0 ; i < hdlcByteToWrite ; i++)
    {
        Serial.write(hdlc_send_framebuffer[i]);
    }
    INIT_TABLE_TO_ZERO(hdlc_send_framebuffer);
}

unsigned int TeleopThread::feedReadBuffer()
{
    while (Serial.available())
    {
        //As the yahdlc layer requires a flat buffer we "un-ring" the driver ring buffer
        serial_recv_buffer[serial_index] = (char) Serial.read();
        serial_index++;
        ardAssert(serial_index <= SERIAL_BUF_SIZE, "Serial framebuffer overshoot");
    }

    return serial_index;
}

void TeleopThread::run()
{
    //use this function to shortcut the google protobuf layer in case you want a simple way of commanding the robot.
    //simpleSerialRun();

    //hldc local vars
    yahdlc_control_t control;

    size_t nbBytesRead = 0;//nb bytes in the receive buffer
    size_t nbHdlcBytes = 0;//nb bytes parsed by yahdlc that can be thrown away
    size_t hdlc_length = 0;

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
                if (nbHdlcBytes == SERIAL_BUF_SIZE)
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

void TeleopThread::handleMsg(char const * msg, size_t msgLength)
{
    apb_TeleopRequest request = apb_TeleopRequest_init_default;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer((const pb_byte_t *)msg, msgLength);

    /* Now we are ready to decode the message. */
    if (!pb_decode(&stream, apb_TeleopRequest_fields, &request))
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

void TeleopThread::getOsStats(apb_TeleopRequest const & request)
{
    g_ArdOs.displayStats();
}

void TeleopThread::configureMatch(apb_TeleopRequest const & request)
{

    uint8_t strategy = request.type.configureMatch.strategy;
    eColor color = (eColor)(request.type.configureMatch.matchColor);
    ROBOT.strategy.configureMatch(strategy, color);
    events[EVT_CONFIGURE].publish();
}

void TeleopThread::startMatch(apb_TeleopRequest const & request)
{
    events[EVT_START_MATCH].publish();
}

void TeleopThread::setPosition(apb_TeleopRequest const & request)
{
    ROBOT.nav.setPosition(
            request.type.setPosition.x,
            request.type.setPosition.y,
            request.type.setPosition.h);
}

void TeleopThread::requestGoto(apb_TeleopRequest const & request)
{
    ROBOT.nav.goTo(
            request.type.requestGoto.target.x,
            request.type.requestGoto.target.y,
            request.type.requestGoto.direction);
}

void TeleopThread::requestGotoCap(apb_TeleopRequest const & request)
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

void TeleopThread::log(LogMsg const & log)
{
    apb_TeleopResponse response = apb_TeleopResponse_init_default;

    /* Create a stream that will write to our buffer. */
    pb_ostream_t stream = pb_ostream_from_buffer((unsigned char*)msg_send_buffer, sizeof(msg_send_buffer));
    response.which_type = apb_TeleopResponse_log_tag;
    response.type.log.date  = log.date;
    response.type.log.level = log.level;
    strcpy(response.type.log.text, log.text.c_str());

    /* Now we are ready to encode the message! */
    ardAssert(pb_encode(&stream, apb_TeleopResponse_fields, &response), "Failed to encode Log message.");
    writeMsg(stream.bytes_written);
}

