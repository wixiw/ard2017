/*
 * RemoteControlThread.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_TELEOPTHREAD_H_
#define ROBOTS_TELEOPTHREAD_H_

#include "RSP.h"
#include "RemoteControl.pb.h"
#include "com/ComInterfaces.h"

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
     * We choosed not to set up a telemetry thread which would provide precise period or event-based data,
     * in order to save embedded ressource. The partner SW is responsible for polling telemetry
     */
    class RemoteControl: public ILogChannel, public IComListener
    {
    public:
        RemoteControl();
        virtual ~RemoteControl() = default;

        //Get any teleop event (non-const on purpose)
        IEvent* getEvent(eRemoteControlEvtId id);

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


    private:
        Event<1> events[EVT_MAX];
        
        ComOnUart com;
        char msg_send_buffer[MSG_SIZE];

        /**------------------------------
         * Receive COM API
         --------------------------------*/
        void getOsStats             (apb_RemoteControlRequest const & request);
        void getOsStatsLogs         (apb_RemoteControlRequest const & request);
        void getTelemetry           (apb_RemoteControlRequest const & request);
        void reboot                 (apb_RemoteControlRequest const & request);
        void configureMatch         (apb_RemoteControlRequest const & request);
        void startMatch             (apb_RemoteControlRequest const & request);
        void setPosition            (apb_RemoteControlRequest const & request);
        void requestGoto            (apb_RemoteControlRequest const & request);
        void requestGotoCap         (apb_RemoteControlRequest const & request);
        void requestMaxLengthMsg    (apb_RemoteControlRequest const & request);

    };

} /* namespace ard */

#endif /* ROBOTS_TELEOPTHREAD_H_ */
