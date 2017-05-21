/*
 * RemoteControlThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "RemoteControl.h"

#ifdef BUILD_MOTION_CONTROL

using namespace ard;

void UART_Handler(void)
{
    //DEBUG_SET_HIGH(); //uncomment to check period and delay with oscilloscope
    RemoteControl::instance->serialDriver.IrqHandler();
    //DEBUG_SET_LOW(); //uncomment to check period and delay with oscilloscope : default empty duration (2 io write + 1 function call) is 750ns
}

#define HANDLE_MSG(msg)                 \
case apb_RemoteControlRequest_##msg##_tag:     \
{                                       \
    msg(request);                        \
    break;                              \
}

RemoteControl* RemoteControl::instance = NULL;

RemoteControl::RemoteControl(Robot2017& robot):
    Robot2017Listener(),
    serialDriver(UART, ID_UART, SERIAL_BUF_SIZE /*RX bvuf size*/, SERIAL_BUF_SIZE /*TX bvuf size*/),
    com("RemCtl", serialDriver, LOG_QUEUE_SIZE),
    robot(robot)
{
    ASSERT_TEXT(instance == NULL, "Only one instance of RemoteControl is allowed.");
    instance = this;
    INIT_TABLE_TO_ZERO(msg_send_buffer);
    serialDriver.setInterruptPriority            (PRIORITY_IRQ_UART0);
    com.setListener(this);
    robot.log.addLogger(*this);
}

void RemoteControl::start()
{
    serialDriver.start(SERIAL_BAUDRATE, SerialMode_8E1 | UART_MR_CHMODE_NORMAL);
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
        HANDLE_MSG(requestPlaySound)
        HANDLE_MSG(getConfig)
        HANDLE_MSG(setConfig)
        HANDLE_MSG(getComStatsLogs)
        HANDLE_MSG(getSerial)
        HANDLE_MSG(configureMatch)
        HANDLE_MSG(startMatch)
        HANDLE_MSG(requestActuators)
        HANDLE_MSG(setPosition)
        HANDLE_MSG(setSpeedAcc)
        HANDLE_MSG(requestGoto)
        HANDLE_MSG(requestGotoCap)
        HANDLE_MSG(requestGoForward)
        HANDLE_MSG(requestTurnDelta)
        HANDLE_MSG(requestTurnTo)
        HANDLE_MSG(requestFaceTo)
        HANDLE_MSG(recalFaceOnBorder)
        HANDLE_MSG(recalRearOnBorder)
        HANDLE_MSG(requestGraphTo)
        HANDLE_MSG(requestBlockRobot)
        HANDLE_MSG(requestMotionGraph)
        HANDLE_MSG(requestMaxLengthMsg)
        HANDLE_MSG(requestCrcFailMsg)
        HANDLE_MSG(requestTooLittleMsg)

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
    LOG_INFO(String("           (including stack : ") + STACK_TOTAL + "o)");
    LOG_INFO(String("  Nb mutexes   : ") + Mutex::getOsObjectCount());
    LOG_INFO(String("  Nb signals   : ") + Signal::getOsObjectCount());
    LOG_INFO(String("  Nb queues    : ") + Queue::getOsObjectCount());
    LOG_INFO(String("  Nb threads   : ") + Thread::getOsObjectCount());
    LOG_INFO(String("  Nb SW Timers : ") + SwTimer::getOsObjectCount());
    LOG_INFO("-----------------------------------------");
}

void RemoteControl::getComStatsLogs(apb_RemoteControlRequest const & request)
{
    LOG_INFO("------------------------------------------------");
    LOG_INFO("|           Serial 0 Stats                     |");
    LOG_INFO("------------------------------------------------");
    LOG_INFO(String(" ########  ##     ##       Total bytes : ") + serialDriver.nbTxBytes);
    LOG_INFO(String(" ##     ##  ##   ##          Total msg : ") + com.getNbRxMsg());
    LOG_INFO(String(" ##     ##   ## ##          Bytes lost : ") + serialDriver.nbRxBytesLost);
    LOG_INFO(String(" ########     ###      Max buffer load : ") + com.getMaxRxBufferLoad());
    LOG_INFO(String(" ##   ##     ## ##    Max msg in queue : ") + com.getMaxRxRawMsg());
    LOG_INFO(String(" ##    ##   ##   ##     Max HDLC payld : ") + com.getMaxRxHdlcPayload());
    LOG_INFO(String(" ##     ## ##     ##           Dropped : ") + com.getHdlcDropMsgCount());
    LOG_INFO("");
    LOG_INFO(String("  ######## ##     ##       Total bytes : ") + serialDriver.nbRxBytes);
    LOG_INFO(String("     ##     ##   ##          Total msg : ") + com.getNbTxMsg());
    LOG_INFO(String("     ##      ## ##          Bytes lost : ") + serialDriver.nbTxBytesLost);
    LOG_INFO(String("     ##       ###      Max buffer load : ") + com.getMaxTxMsgCount());
    LOG_INFO(String("     ##      ## ##     Max raw  length : ") + com.getMaxTxMsgLength());
    LOG_INFO(String("     ##     ##   ##     Max HDLC payld : ") + com.getMaxTxHdlcPayload());
    LOG_INFO(String("     ##    ##     ##                     "));
    LOG_INFO("");
    LOG_INFO(String("  FrameError        : ") + serialDriver.nbFrameError);
    LOG_INFO(String("  ParityError       : ") + serialDriver.nbParityError);
    LOG_INFO(String("  MissedIrq         : ") + serialDriver.nbMissedIrq);
    LOG_INFO(String("  HDLC Frame length : ") + HDLC_FRAME_LENGTH);
    LOG_INFO(String("  HDLC buffer size  : ") + SERIAL_BUF_SIZE);
    LOG_INFO(String("  LOG_QUEUE_SIZE    : ") + LOG_QUEUE_SIZE);
    LOG_INFO("-----------------------------------------");
}

void RemoteControl::getTelemetry(apb_RemoteControlRequest const & request)
{
    {
        /* Clean send buffer and create a stream that will write to our buffer. */
        INIT_TABLE_TO_ZERO(msg_send_buffer);
        pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t*) msg_send_buffer, sizeof(msg_send_buffer));

        /* populates message */
        apb_RemoteControlResponse response = apb_RemoteControlResponse_init_default;
        response.which_type                     = apb_RemoteControlResponse_telemetry_tag;
        response.type.telemetry.date            = millis();
        response.type.telemetry.nav             = robot.nav.serealize();
        response.type.telemetry.actuators       = robot.actuators.serealize();
        response.type.telemetry.stratInfo       = robot.stratInfo.serialize();
        response.type.telemetry.hmi             = robot.hmi.serealize();
        response.type.telemetry.chrono          = robot.chrono.serialize();

        /* Now we are ready to encode the message! */
        bool res = pb_encode(&stream, apb_RemoteControlResponse_fields, &response);
        ASSERT_TEXT(res, stream.errmsg);
        ASSERT_TEXT(com.sendMsg(msg_send_buffer, stream.bytes_written), "RemoteControl: get telemetry failed");
    }

    //As all graph data doesn't fit in the telemetry message, it's better to send an independent message.
    //Only send the graph info if requested explicitely
    if(request.type.getTelemetry)
    {
        /* Clean send buffer and create a stream that will write to our buffer. */
        INIT_TABLE_TO_ZERO(msg_send_buffer);
        pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t*) msg_send_buffer, sizeof(msg_send_buffer));

        /* populates message */
        apb_RemoteControlResponse response  = apb_RemoteControlResponse_init_default;
        response.which_type                 = apb_RemoteControlResponse_graphState_tag;
        response.type.graphState            = robot.motionGraph.serializeState();

        /* Now we are ready to encode the message! */
        bool res = pb_encode(&stream, apb_RemoteControlResponse_fields, &response);
        ASSERT_TEXT(res, stream.errmsg);
        ASSERT_TEXT(com.sendMsg(msg_send_buffer, stream.bytes_written), "RemoteControl: get graph state failed");
    }
}

void RemoteControl::reboot(apb_RemoteControlRequest const & request)
{
    ArdOs::reboot();
}

void RemoteControl::requestPlaySound(apb_RemoteControlRequest const & request)
{
    LOG_INFO("Playing melody...");
    for(unsigned int i = 0 ; i < request.type.requestPlaySound.tones_count ; i++)
    {
        apb_Tone tone = request.type.requestPlaySound.tones[i];
        robot.hmi.buzzer.playTone(tone.frequency, tone.duration);
    }
}

void RemoteControl::getConfig(apb_RemoteControlRequest const & request)
{
    /* Clean send buffer and create a stream that will write to our buffer. */
    INIT_TABLE_TO_ZERO(msg_send_buffer);
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t*) msg_send_buffer, sizeof(msg_send_buffer));

    /* populates message */
    apb_RemoteControlResponse response  = apb_RemoteControlResponse_init_default;
    response.which_type                 = apb_RemoteControlResponse_config_tag;
    response.type.config                = robot.getConfig();

    /* Now we are ready to encode the message! */
    bool res = pb_encode(&stream, apb_RemoteControlResponse_fields, &response);
    ASSERT_TEXT(res, stream.errmsg);
    ASSERT_TEXT(com.sendMsg(msg_send_buffer, stream.bytes_written), "RemoteControl: get config failed");
}

void RemoteControl::setConfig(apb_RemoteControlRequest const & request)
{
    robot.setConfig(request.type.setConfig);
    getConfig(request);
}

void RemoteControl::getSerial(apb_RemoteControlRequest const & request)
{
    bootUp();
}


void RemoteControl::configureMatch(apb_RemoteControlRequest const & request)
{
    uint8_t strategy = request.type.configureMatch.strategy;
    eColor color = (eColor)(request.type.configureMatch.matchColor);
    bool simulated = request.type.configureMatch.simulated;

    robot.lifecycle.networkConfigRequest(strategy, color, simulated);
}

void RemoteControl::startMatch(apb_RemoteControlRequest const & request)
{
    robot.lifecycle.networkStartRequest();
}

void RemoteControl::requestActuators(apb_RemoteControlRequest const & request)
{
    if( request.type.requestActuators.hasLifter )
        robot.actuators.servoLifter.goTo(request.type.requestActuators.lifter);
    if( request.type.requestActuators.hasLeftArm )
        robot.actuators.servoLeftArm.goTo(request.type.requestActuators.leftArm);
    if( request.type.requestActuators.hasRightArm )
        robot.actuators.servoRightArm.goTo(request.type.requestActuators.rightArm);
    if( request.type.requestActuators.hasLeftWheel )
        robot.actuators.servoLeftWheel.goTo(request.type.requestActuators.leftWheel);
    if( request.type.requestActuators.hasRightWheel )
        robot.actuators.servoRightWheel.goTo(request.type.requestActuators.rightWheel);
    if( request.type.requestActuators.hasFunnyAction )
        robot.actuators.servoFunnyAction.goTo(request.type.requestActuators.funnyAction);
    if( request.type.requestActuators.hasHighLevelCmd )
    	robot.actuators.actCmd(request.type.requestActuators.highLevelCmd);
}

void RemoteControl::setPosition(apb_RemoteControlRequest const & request)
{
    robot.nav.setPosition(Pose2D::fromProto(request.type.setPosition));
}

void RemoteControl::setSpeedAcc(apb_RemoteControlRequest const & request)
{
    robot.kinematics.setSpeedAcc(
            request.type.setSpeedAcc.vMax,
            request.type.setSpeedAcc.vMaxTurn,
            request.type.setSpeedAcc.accMax,
            request.type.setSpeedAcc.accMaxTurn);
}

void RemoteControl::requestGoto(apb_RemoteControlRequest const & request)
{
    robot.nav.goTo(
            request.type.requestGoto.target.x,
            request.type.requestGoto.target.y,
            request.type.requestGoto.direction);
}

void RemoteControl::requestGotoCap(apb_RemoteControlRequest const & request)
{
    robot.nav.goToCap(
            request.type.requestGotoCap.target.x,
            request.type.requestGotoCap.target.y,
            request.type.requestGotoCap.target.h,
            request.type.requestGotoCap.direction);
}

void RemoteControl::requestGoForward(apb_RemoteControlRequest const & request)
{
    robot.nav.goForward(request.type.requestGoForward);
}

void RemoteControl::requestTurnDelta(apb_RemoteControlRequest const & request)
{
    robot.nav.turnDelta(request.type.requestTurnDelta);
}

void RemoteControl::requestTurnTo(apb_RemoteControlRequest const & request)
{
    robot.nav.turnTo(request.type.requestTurnTo);
}

void RemoteControl::requestFaceTo(apb_RemoteControlRequest const & request)
{
    robot.nav.faceTo(Point(request.type.requestFaceTo.x,
                      request.type.requestFaceTo.y));
}

void RemoteControl::recalFaceOnBorder(apb_RemoteControlRequest const & request)
{
    robot.nav.recalFace(request.type.recalFaceOnBorder);
}

void RemoteControl::recalRearOnBorder(apb_RemoteControlRequest const & request)
{
    robot.nav.recalRear(request.type.recalRearOnBorder);
}

void RemoteControl::requestGraphTo(apb_RemoteControlRequest const & request)
{
    robot.nav.graphTo(request.type.requestGraphTo.target.x,
                      request.type.requestGraphTo.target.y,
                      request.type.requestGraphTo.target.h,
                      request.type.requestGraphTo.direction);
}

void RemoteControl::requestBlockRobot(apb_RemoteControlRequest const & request)
{
    if(request.type.requestBlockRobot)
        LOG_INFO("Robot block request.");
    else
        LOG_INFO("Robot unblock request.");
    robot.detection.fakeRobot = request.type.requestBlockRobot;
}

void RemoteControl::requestMotionGraph(apb_RemoteControlRequest const & request)
{
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t*) msg_send_buffer, sizeof(msg_send_buffer));

    /* Send NODES */
    INIT_TABLE_TO_ZERO(msg_send_buffer);
    apb_RemoteControlResponse response  = apb_RemoteControlResponse_init_default;
    response.which_type                 = apb_RemoteControlResponse_graphNodes_tag;
    robot.motionGraph.serializeNodes(response.type.graphNodes);
    bool res = pb_encode(&stream, apb_RemoteControlResponse_fields, &response);
    ASSERT_TEXT(res, stream.errmsg);
    ASSERT_TEXT(com.sendMsg(msg_send_buffer, stream.bytes_written), "RemoteControl: get graph failed (nodes)");

    /* Send LINKS */
    INIT_TABLE_TO_ZERO(msg_send_buffer);
    stream = pb_ostream_from_buffer((pb_byte_t*) msg_send_buffer, sizeof(msg_send_buffer));
    memset(&response, 0, sizeof(apb_RemoteControlResponse));
    response.which_type                 = apb_RemoteControlResponse_graphLinks_tag;
    robot.motionGraph.serializeLinks(response.type.graphLinks);
    res = pb_encode(&stream, apb_RemoteControlResponse_fields, &response);
    ASSERT_TEXT(res, stream.errmsg);
    ASSERT_TEXT(com.sendMsg(msg_send_buffer, stream.bytes_written), "RemoteControl: get graph failed (links)");
}

void RemoteControl::requestMaxLengthMsg(apb_RemoteControlRequest const & request)
{
    char buf[502];
    for(int i = 0; i < 256; i++)
    {
        buf[i] = i;
    }
    for(int i = 0; i < 246; i++)
    {
        buf[255 + i] = i;
    }

    ASSERT_TEXT(com.sendMsg(buf, 502), "RemoteControl: requestMaxLengthMsg failed");
}

void RemoteControl::requestCrcFailMsg(apb_RemoteControlRequest const & request)
{
    char buf[6] = { '~', 0xFF, 0x10, 6, 0xE1, '~' };
    ASSERT_TEXT(com.sendMsg(buf, 6), "RemoteControl: requestCrcFailMsg failed");
}

void RemoteControl::requestTooLittleMsg(apb_RemoteControlRequest const & request)
{
    char buf[4] = { '~', 0xFF, 0x10, '~' };
    ASSERT_TEXT(com.sendMsg(buf, 4), "RemoteControl: requestTooLittleMsg failed");
}


/**
 * Send COM API
 */

void RemoteControl::log(LogMsg const & log)
{
    mutex.lock();

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
    bool res = pb_encode(&stream, apb_RemoteControlResponse_fields, &response);
    ASSERT_TEXT(res, stream.errmsg);
    ASSERT_TEXT(com.sendMsg(msg_send_buffer, stream.bytes_written), "RemoteControl: log failed");

    mutex.unlock();
}

void RemoteControl::bootUp()
{
    mutex.lock();

    /* Clean send buffer and create a stream that will write to our buffer. */
    INIT_TABLE_TO_ZERO(msg_send_buffer);
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t*) msg_send_buffer, sizeof(msg_send_buffer));

    /* populates message */
    apb_RemoteControlResponse response = apb_RemoteControlResponse_init_default;
    response.which_type         = apb_RemoteControlResponse_serialNumber_tag;
    strcpy(response.type.serialNumber.value, robot.getSerialNumber());

    /* Now we are ready to encode the message! */
    bool res = pb_encode(&stream, apb_RemoteControlResponse_fields, &response);
    ASSERT_TEXT(res, stream.errmsg);
    ASSERT_TEXT(com.sendMsg(msg_send_buffer, stream.bytes_written), "RemoteControl: serial number send failed");

    mutex.unlock();
}

#endif //BUILD_MOTION_CONTROL
