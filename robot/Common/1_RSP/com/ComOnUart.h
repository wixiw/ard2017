/*
 * Communication.h
 *
 *  Created on: 10 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_COMMUNICATION_H_
#define ROBOT_COMMON_1_RSP_COMMUNICATION_H_

#include "ComInterfaces.h"
#include "ArdHdlc.h"

namespace ard
{
    /**
     * This class is the Top Level class to use to setup an UART communication with HDLC.
     * It provides threads to handle send/receive commands, and assembles all the communication layers
     */
    class ComOnUart: public ICom
    {
    public:
        ComOnUart(String name);
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
        //@param : return false if send queue is full : retry later
        virtual bool sendMsg(char const * msg, size_t msgLength) override;

    private:
        /**
         * This internal class is used to manage the emission thread
         */
        class Sender //TODO Thread
        {
        public:
            Sender(ComOnUart& parent);
            virtual ~Sender() = default;

            //Override Thread : creates the thread
            void init(); //TODO override;

            //Implements Thread : reads the serial inputs
            void run(); //TODO override;

        private:
            ComOnUart& parent;
//            QueueHandle_t queue;
//            StaticQueue_t _queue_private_state;
//#define QUEUE_LENGTH 3
//#define QUEUE_ITEM_SIZE 2
//            uint8_t queueBuffer[QUEUE_LENGTH*QUEUE_ITEM_SIZE];
        };

        /**
         * This internal class is used to manage the reception thread
         */
        class Receiver //TODO Thread
        {
        public:
            Receiver(ComOnUart& parent);
            virtual ~Receiver() = default;

            //Override Thread : creates the thread
            void init(); //TODO override;

            //Implements Thread : reads the serial inputs
            void run(); //TODO override;

        private:
            ComOnUart& parent;
        };

        String name;
        ArdHdlc dataLink;
        Sender txThread;
        Receiver rxThread;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_1_RSP_COMMUNICATION_H_ */
