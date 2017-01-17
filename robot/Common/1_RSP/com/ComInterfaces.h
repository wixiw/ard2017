/*
 * ComInterfaces.h
 *
 *  Created on: 10 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_COM_COMINTERFACES_H_
#define ROBOT_COMMON_1_RSP_COM_COMINTERFACES_H_

#include "ArdOs.h"

namespace ard
{
    /**
     * Decoupling listener interface for ICom
     */
    class ICom;
    class IComListener
    {
    public:
        virtual ~IComListener();
        virtual void com_connected(ICom const* origin) = 0;
        virtual void com_disconnected(ICom const* origin) = 0;

        //A message has been received from the communication media
        //@param origin : the ICom interface that is calling the method
        //@param msg : the data to decode
        //@param msgLength : the length of the data buffer
        virtual void handleMsg(ICom const* origin, char const * msg, size_t msgLength) = 0;
    };

    /**
     * This class is the interface to use for any communication stack.
     */
    class ICom
    {
    public:
        virtual ~ICom();
        virtual void setListener(IComListener* newListener) = 0;

        //Open the communication stack.
        //You shoudld check that isStarted() is true before sending messages
        virtual bool connect() = 0;

        //Close the communication stack
        virtual void disconnect() = 0;

        //@returns true if the Communication is up and ready to communicate
        virtual bool isConnected() const = 0;

        //Write a message on the communication media
        virtual bool sendMsg(char const * msg, size_t msgLength) = 0;
    };
}

#endif /* ROBOT_COMMON_1_RSP_COM_COMINTERFACES_H_ */
