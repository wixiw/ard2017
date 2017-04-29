/*
 * RemoteControlThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_TELEOPTHREAD_H_
#define ROBOTS_TELEOPTHREAD_H_

#include <Com.h>
#include "RSP.h"
#include "RemoteControl.pb.h"
#include "ComInterfaces.h"

namespace ard
{
    typedef enum
    {
        EVT_CONFIGURE = 0,
        EVT_START_MATCH,
        EVT_MAX
    } eRemoteControlEvtId;

#ifdef BUILD_REMOTE_CONTROL

    //forward declaration
    class Robot2017;

    /**
     * This class is used to rwait and receive commands from debug serial link
     * The aim of those commands are :
     * - simulate an HW event
     * - teleoperate the robot for tests
     * We choosed not to set up a telemetry thread which would provide precise period or event-based data,
     * in order to save embedded ressource. The partner SW is responsible for polling telemetry
     */
    class RemoteControl: public ILogChannel, public IComListener
    {
    public:
        RemoteControl(Robot2017* newRobot, ISerialDriver& serialDriver);
        virtual ~RemoteControl() = default;

        //Implements ILogChannel : returns true if the communication is established
        virtual bool isReady() const override;

        //Implements IComListener : not used
        virtual void com_connected(ICom const* origin) override{};

        //Implements IComListener : not used
        virtual void com_disconnected(ICom const* origin) override{};

        //Implements IComListener : parse the message and route it the the "Receive COM API" methods
        void handleMsg(ICom const* origin, char const * msg, size_t msgLength) override;

        /**------------------------------
         * Send COM API
         --------------------------------*/
        //Implements ILogChannel : push a log on the serial link
        virtual void log(LogMsg const & log) override;

        //Send serial number on communication link
        void sendSerialNumber();

    private:
        Mutex mutex;
        ComOnUart com;
        Robot2017* robot;
        char msg_send_buffer[HDLC_FRAME_LENGTH];

        /**------------------------------
         * Receive COM API
         --------------------------------*/
        void getOsStats             (apb_RemoteControlRequest const & request);
        void getOsStatsLogs         (apb_RemoteControlRequest const & request);
        void getTelemetry           (apb_RemoteControlRequest const & request);
        void reboot                 (apb_RemoteControlRequest const & request);
        void requestPlaySound       (apb_RemoteControlRequest const & request);
        void getConfig              (apb_RemoteControlRequest const & request);
        void setConfig              (apb_RemoteControlRequest const & request);
        void getComStatsLogs        (apb_RemoteControlRequest const & request);
        void getSerial              (apb_RemoteControlRequest const & request);
        void configureMatch         (apb_RemoteControlRequest const & request);
        void startMatch             (apb_RemoteControlRequest const & request);
        void requestActuators       (apb_RemoteControlRequest const & request);
        void setPosition            (apb_RemoteControlRequest const & request);
        void setSpeedAcc            (apb_RemoteControlRequest const & request);
        void requestGoto            (apb_RemoteControlRequest const & request);
        void requestGotoCap         (apb_RemoteControlRequest const & request);
        void requestGoForward       (apb_RemoteControlRequest const & request);
        void requestTurnDelta       (apb_RemoteControlRequest const & request);
        void requestTurnTo          (apb_RemoteControlRequest const & request);
        void requestFaceTo          (apb_RemoteControlRequest const & request);
        void recalFaceOnBorder      (apb_RemoteControlRequest const & request);
        void recalRearOnBorder      (apb_RemoteControlRequest const & request);
        void requestBlockRobot      (apb_RemoteControlRequest const & request);
        void requestMaxLengthMsg    (apb_RemoteControlRequest const & request);
        void requestCrcFailMsg      (apb_RemoteControlRequest const & request);
        void requestTooLittleMsg    (apb_RemoteControlRequest const & request);

    };

#endif //BUILD_REMOTE_CONTROLs

} /* namespace ard */



#endif /* ROBOTS_TELEOPTHREAD_H_ */