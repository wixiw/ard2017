/*
 * ComOnUart.cpp
 *
 *  Created on: 10 janv. 2017
 *      Author: wix
 */

#include "ComOnUart.h"

using namespace ard;

ComOnUart::ComOnUart(String name):
        name(name),
        dataLink(name, Serial),
        txThread(*this),
        rxThread(*this)
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

bool ComOnUart::sendMsg(char const * msg, size_t msgLength)
{
    //TODO : faire une queue de message pour envoyer
    return dataLink.sendMsg(msg, msgLength);
}

ComOnUart::Sender::Sender(ComOnUart& _parent)
    : parent(_parent)
{
}

void ComOnUart::Sender::init()
{
    //create the thread
//    g_ArdOs.createThread_Cpp("RemoteControlTx", *this, STACK_TELEOP_TX, PRIO_TELEOP_TX);
//    queue = xQueueCreateStatic(QUEUE_LENGTH, QUEUE_ITEM_SIZE, queueBuffer, _queue_private_state);
}

void ComOnUart::Sender::run()
{
    //TODO lire la pile d'entree et depiler sur le bus. Attention a bien tester que le driver est dispo
    //assert (dataLink.sendMsg(msg, msgLength), "Sender::run issue");
}

ComOnUart::Receiver::Receiver(ComOnUart& parent)
    : parent(parent)
{
}

void ComOnUart::Receiver::init()
{
    //create the thread
//    g_ArdOs.createThread_Cpp("RemoteControlRx", *this, STACK_TELEOP_RX, PRIO_TELEOP_RX);
}

void ComOnUart::Receiver::run()
{
    while (2)
    {
        //There are a lot of overflow and performance issues in the following algorithm, but it's a starting point (anyway the Arduino buffer is worse ...)
        //if no bytes is present wait a bit and read again
        if ( parent.dataLink.feedReadBuffer())
        {
            parent.dataLink.parseBuffer();
            vTaskDelay(10);   
            
        }
        else
        {
            vTaskDelay(50);
        }
    } //end while(2)
}

