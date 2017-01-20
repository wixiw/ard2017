/*
 * ComOnUart.cpp
 *
 *  Created on: 10 janv. 2017
 *      Author: wix
 */

#include "ComOnUart.h"

using namespace ard;

ComOnUart::ComOnUart(String const& name, uint8_t recvQueueSize):
        ArdObject(name),
        dataLink(name, Serial),
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
        com(com),
        queueLength(recvQueueSize, sizeof(size_t)),
        queueMsg(recvQueueSize, MSG_SIZE)
{
}

void ComOnUart::Sender::run()
{
    char sendBuf[MSG_SIZE];

    while(2)
    {
        size_t length = 0;
        queueLength.pop(&length);
        queueMsg.pop(&sendBuf);
        ASSERT_TEXT(com.sendMsg(sendBuf, length), "Sender::run issue");
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
    while (2)
    {
        //There are a lot of overflow and performance issues in the following algorithm, but it's a starting point (anyway the Arduino buffer is worse ...)
        //if no bytes is present wait a bit and read again
        if ( com.feedReadBuffer())
        {
            com.parseBuffer();
            vTaskDelay(10);   
            
        }
        else
        {
            vTaskDelay(50);
        }
    } //end while(2)
}

