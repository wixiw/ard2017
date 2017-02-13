/*
 * RemoteControlThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "RemoteControl.h"
#include "Robot2017.h"

using namespace ard;

#define HANDLE_MSG(msg)                 \
case apb_RemoteControlRequest_##msg##_tag:     \
{                                       \
    LOG_DEBUG(#msg " request received."); \
    msg(request);                        \
    break;                              \
}

RemoteControl::RemoteControl():
    com("RemCtl", LOG_QUEUE_SIZE)
{
    INIT_TABLE_TO_ZERO(msg_send_buffer);
    com.setListener(this);
}

IEvent* RemoteControl::getEvent(eRemoteControlEvtId id)
{
    return &events[id];
}

bool RemoteControl::isReady() const
{
    return com.isConnected();
}

void RemoteControl::handleMsg(ICom const* origin, char const * msg, size_t msgLength)
{
    UNUSED(origin);
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
        HANDLE_MSG(getOsStatsLogs)
        HANDLE_MSG(getTelemetry)
        HANDLE_MSG(reboot)
        HANDLE_MSG(configureMatch)
        HANDLE_MSG(startMatch)
        HANDLE_MSG(setPosition)
        HANDLE_MSG(requestGoto)
        HANDLE_MSG(requestGotoCap)
        HANDLE_MSG(requestMaxLengthMsg)


        default:
        {
            LOG_ERROR("Failed to identify message type : " + String((int)request.which_type));
            //message is sent back to the sender for analysis
            ASSERT(com.sendMsg(msg, msgLength));
            break;
        }
    }

}

/**
 * Receive COM API
 */

void RemoteControl::getOsStats(apb_RemoteControlRequest const & request)
{
    ASSERT(false);
}

void RemoteControl::getOsStatsLogs(apb_RemoteControlRequest const & request)
{
    LOG_INFO("------------ ArdOs Stats  ---------------");
    LOG_INFO("|   Thread   |  used stack (in words)   |");
    LOG_INFO("-----------------------------------------");

    Thread::ThreadParams const* threads = Thread::getThreadParams();

    LOG_INFO(String("  Idle\t")
            + uxTaskGetStackHighWaterMark(xTaskGetIdleTaskHandle())
            + "/" + configMINIMAL_STACK_SIZE);

    /* Create a human readable table from the binary data. */
    for( int i = 1; i < PRIO_NB; i++ )
    {
        if(threads[i].handle != NULL)
        {
            LOG_INFO(String("  ") + pcTaskGetTaskName(threads[i].handle)
                    + "\t" + String(threads[i].stackSize - uxTaskGetStackHighWaterMark(threads[i].handle))
                    + "/" + threads[i].stackSize
                    + " ( " + String((100*(threads[i].stackSize - uxTaskGetStackHighWaterMark(threads[i].handle))) / threads[i].stackSize) + "%)");
        }
    }

    LOG_INFO("-----------------------------------------");
    LOG_INFO("| CPU consumption                       |");
    LOG_INFO("-----------------------------------------");
    static char buf[1+40*PRIO_NB];
    buf[0] = '\n'; //return to new line
    vTaskGetRunTimeStats(buf+1);
    LOG_INFO(buf);

    LOG_INFO("-----------------------------------------");
    LOG_INFO(String("  Heap         : ") + (100*(configTOTAL_HEAP_SIZE - xPortGetFreeHeapSize()))/configTOTAL_HEAP_SIZE + "% (" + String(configTOTAL_HEAP_SIZE - xPortGetFreeHeapSize()) + "o / " + configTOTAL_HEAP_SIZE + "o)");
    LOG_INFO(String("  Nb mutexes   : ") + Mutex::getOsObjectCount());
    LOG_INFO(String("  Nb signals   : ") + Signal::getOsObjectCount());
    LOG_INFO(String("  Nb queues    : ") + Queue::getOsObjectCount());
    LOG_INFO(String("  Nb threads   : ") + Thread::getOsObjectCount());
    LOG_INFO(String("  Nb SW Timers : ") + SwTimer::getOsObjectCount());
    LOG_INFO("-----------------------------------------");
}

void RemoteControl::getTelemetry(apb_RemoteControlRequest const & request)
{
    /* Clean send buffer and create a stream that will write to our buffer. */
    INIT_TABLE_TO_ZERO(msg_send_buffer);
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t*) msg_send_buffer, sizeof(msg_send_buffer));

    /* populates message */
    apb_RemoteControlResponse response = apb_RemoteControlResponse_init_default;
    response.which_type                     = apb_RemoteControlResponse_telemetry_tag;
    response.type.telemetry.date            = millis();
    response.type.telemetry.nav             = ROBOT.nav.getState();
    response.type.telemetry.actuators       = ROBOT.actuators.getState();

    /* Now we are ready to encode the message! */
    ASSERT_TEXT(pb_encode(&stream, apb_RemoteControlResponse_fields, &response), "Failed to encode Log message.");
    ASSERT_TEXT(com.sendMsg(msg_send_buffer, stream.bytes_written), "RemoteControl: get telemetry failed");
}

void RemoteControl::reboot(apb_RemoteControlRequest const & request)
{
    ArdOs::reboot();
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
    ROBOT.nav.setPosition(PointCap::fromProto(request.type.setPosition));
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

void RemoteControl::requestMaxLengthMsg(apb_RemoteControlRequest const & request)
{
    char buf[510];
    for(int i = 0; i < 256; i++)
    {
        buf[i] = i;
        if( i < 255 )
        {
            buf[2*i] = i;
        }
    }
    buf[509] = 253;

    ASSERT_TEXT(com.sendMsg(buf, 510), "RemoteControl: requestMaxLengthMsg failed");
}


/**
 * Send COM API
 */

void RemoteControl::log(LogMsg const & log)
{
    /* Clean send buffer and create a stream that will write to our buffer. */
    INIT_TABLE_TO_ZERO(msg_send_buffer);
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t*) msg_send_buffer, sizeof(msg_send_buffer));

    /* populates message */
    apb_RemoteControlResponse response = apb_RemoteControlResponse_init_default;
    response.which_type         = apb_RemoteControlResponse_log_tag;
    response.type.log.date      = log.date;
    response.type.log.level     = log.level;
    strcpy(response.type.log.component, log.component);
    strcpy(response.type.log.text, log.text);

    /* Now we are ready to encode the message! */
    ASSERT_TEXT(pb_encode(&stream, apb_RemoteControlResponse_fields, &response), "Failed to encode Log message.");
    ASSERT_TEXT(com.sendMsg(msg_send_buffer, stream.bytes_written), "RemoteControl: log failed");
}


