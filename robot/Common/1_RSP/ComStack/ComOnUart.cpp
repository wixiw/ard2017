/*
 * ComOnUart.cpp
 *
 *  Created on: 10 janv. 2017
 *      Author: wix
 */

#include "ComOnUart.h"

using namespace ard;

ComOnUart::ComOnUart(String const& name, ISerialDriver& serialDriver, uint8_t recvQueueSize):
        ArdObject(name),
        dataLink(name, serialDriver),
        txThread(name+"Tx", dataLink, recvQueueSize),
        rxThread(name+"Rx", dataLink)
{
}

void ComOnUart::init()
{
    txThread.init();
    rxThread.init();
}

void ComOnUart::setListener(IComListener* newListener)
{
    dataLink.setListener(newListener);
}

bool ComOnUart::connect()
{
    return dataLink.connect();
}

void ComOnUart::disconnect()
{
    dataLink.disconnect();
}

bool ard::ComOnUart::isConnected() const
{
    return dataLink.isConnected();
}

ComOnUart::Sender::Sender(String const& name, ICom& com, uint8_t recvQueueSize):
        Thread(name, PRIO_REMOTE_CTRL_TX, STACK_REMOTE_CTRL_TX),
        maxMsgLength(0),
        maxMsgCount(0),
        com(com),
        queueLength(recvQueueSize, sizeof(size_t)),
        queueMsg(recvQueueSize, HDLC_FRAME_LENGTH)
{
}

void ComOnUart::Sender::run()
{
    char sendBuf[HDLC_FRAME_LENGTH];

    while(2)
    {
        size_t length = 0;

        // a little bit of stats
        uint8_t nbMsg = queueMsg.getOsObjectCount();
        if( nbMsg > maxMsgCount) maxMsgCount = nbMsg;

        queueLength.pop(&length);
        queueMsg.pop(&sendBuf);
        ASSERT_TEXT(com.sendMsg(sendBuf, length), "Sender::run issue");
        // keep track of max length for stats
        if( length > maxMsgLength) maxMsgLength = length;

    }
}

bool ComOnUart::Sender::sendMsg(char const * msg, size_t msgLength)
{
    if( queueLength.push(&msgLength, 0) )
    {
        ASSERT(queueMsg.push((void*) msg));
        return true;
    }
    else
    {
        return false;
    }

}

ComOnUart::Receiver::Receiver(String const& name, ArdHdlc& com):
        Thread(name, PRIO_REMOTE_CTRL_RX, STACK_REMOTE_CTRL_RX),
        com(com)
{
}

void ComOnUart::Receiver::run()
{
    while(1)
    {
        com.readAll();
    }
}

