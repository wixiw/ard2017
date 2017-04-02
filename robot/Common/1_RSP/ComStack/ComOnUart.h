/*
 * Communication.h
 *
 *  Created on: 10 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_COMMUNICATION_H_
#define ROBOT_COMMON_1_RSP_COMMUNICATION_H_

#include "BSP.hpp"
#include "ComInterfaces.h"
#include "ArdHdlc.h"

namespace ard
{
    /**
     * This class is the Top Level class to use to setup an UART communication with HDLC.
     * It provides threads to handle send/receive commands, and assembles all the communication layers
     */
    class ComOnUart: public ICom, public ArdObject
    {
    public:
        typedef struct{
            const char msg[HDLC_FRAME_LENGTH];
            size_t length;
        } MsgBuffer;

        /**
         * @param : the prefix for Rx and Tx threads name
         * @param : the reference to the serial driver
         * @param : the length of the queue where log producers pile messages until the TX thread unpiles it
         */
        ComOnUart(String const & name, ISerialDriver& serialDriver, uint8_t recvQueueSize);
        virtual ~ComOnUart() = default;

        //Init threads
        void init();

        //Implements ICom interface to act as a decorator for datalink
        virtual void setListener(IComListener* newListener) override;

        //Implements ICom interface, open the serial link with config set by configure() method
        virtual bool connect() override;

        //Implements ICom interface : close the serial link
        virtual void disconnect() override;

        //Implements ICom interface : check the serial link
        virtual bool isConnected() const override;

        //Implements ICom interface :
        //@return : return false if send queue is full : retry later
        virtual bool sendMsg(char const * msg, size_t msgLength) override
        {
            return txThread.sendMsg(msg, msgLength);
        }

        //Read operation is done by an internal thread
        void readAll() override
        {
            ASSERT(false);
        }

        uint32_t getMaxTxMsgLength() { return txThread.maxMsgLength; }      // max size of message to send BEFORE HDLC serialization (= HDLC payload)
        uint32_t getMaxTxHdlcPayload() { return dataLink.maxTxRawMsg; }     // max size of sent HDLC full msg
        uint32_t getMaxRxBufferLoad() { return dataLink.maxRxBufferLoad; }  // max raw data in serial_recv_buffer
        uint32_t getMaxRxRawMsg() { return dataLink.maxRxRawMsg; }          // max size of single message received BEFORE HDLC parsing
        uint32_t getMaxRxHdlcPayload() { return dataLink.maxRxPayloadSize; }// max size of received HDLC payload

    private:
        /**
         * This internal class is used to manage the emission thread
         */
        class Sender: public Thread
        {
        public:
            Sender(String const& name, ICom& com, uint8_t recvQueueSize);
            virtual ~Sender() = default;

            //Implements Thread : reads the serial inputs
            void run() override;

            //@return : return false if send queue is full : retry later
            bool sendMsg(char const * msg, size_t msgLength);

            uint32_t maxMsgLength;

        private:
            //The com link on which to send data
            ICom& com;

            //One queue for each parameter, it's important to queue and unqueue in the same order.
            Queue queueLength;
            Queue queueMsg;
        };

        /**
         * This internal class is used to manage the reception thread
         */
        class Receiver: public Thread
        {
        public:
            Receiver(String const& name, ArdHdlc& com);
            virtual ~Receiver() = default;

            //Implements Thread : reads the serial inputs
            void run() override;

        private:
            //The com link on which to send data
            ArdHdlc& com;
        };

        String name;
        ArdHdlc dataLink;
        Sender txThread;
        Receiver rxThread;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_COMMUNICATION_H_ */
