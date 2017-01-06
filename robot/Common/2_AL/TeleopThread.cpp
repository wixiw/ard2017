/*
 * TeleopThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "TeleopThread.h"
#include "Robot2017.h"

using namespace ard;

#define SERIAL_BUF_SIZE 200
#define HDLC_BUF_SIZE 200

void TeleopThread::init()
{
    //create the thread
    g_ArdOs.createThread_Cpp("Teleop", *this, STACK_LOG, PRIO_LOG);
}

void TeleopThread::run()
{
    //use this function to shortcut the google protobuf layer in case you want a simple way of commanding the robot.
    //simpleSerialRun();

    //hldc local vars
    yahdlc_control_t control;
    
    char serial_framebuffer[SERIAL_BUF_SIZE];
    unsigned int serial_index = 0;
    unsigned int nbSerialBytes = 0;
    char hdlc_framebuffer[HDLC_BUF_SIZE];
    unsigned int hdlc_length = 0;
    INIT_TABLE_TO_ZERO(serial_framebuffer);
    

    while (2)
    {
        //There are a lot of overflow and performance issues in the following algorithm, but it's a starting point

        while (Serial.available())
        {
            //As the yahdlc layer requires a flat buffer we "un-ring" the driver ring buffer
            serial_framebuffer[serial_index] = (char)Serial.read();
            serial_index++;
            ardAssert(serial_index <= SERIAL_BUF_SIZE, "Serial framebuffer overshoot");
        }
        
        //if no bytes is present wait a bit and read again
        if(serial_index == 0)
        {
            vTaskDelay(10);
            continue;
        }            

        do
        {
            //for clarity of code, this variable is renamed
            nbSerialBytes = serial_index;
            
            //clean the buffer that will receive the future frame
            hdlc_length = 0;
            INIT_TABLE_TO_ZERO(hdlc_framebuffer);
            INIT_STRUCT_TO_ZERO(control);

            //as a simple solution we filter char by char, for performance improvments we could modify the Arduino UARTClass
            //and add a ReadAll function to get a longer buffer. As long as only non-operational action is
            //done here, there is no need for performance
            int res = yahdlc_get_data(&control, serial_framebuffer, nbSerialBytes, hdlc_framebuffer, &hdlc_length);
            if( 0 < res )
            {
                //clean input buffer : unused data at the front and decoded frame are deleted
                int nbBytesRead = res + 1; //the result is the index of the last read element in the buffer
                if(nbBytesRead == SERIAL_BUF_SIZE)
                {
                    INIT_TABLE_TO_ZERO(serial_framebuffer);
                    serial_index = 0;
                }
                else
                {
                    //it's possible that some bytes are present after the decoded frame (ex : several received frames)
                    unsigned int remainingBytesNb = nbSerialBytes - nbBytesRead;
                    char* startOfRemainingBytes = serial_framebuffer + nbBytesRead; //no overflow as full buffer case is treated before
                    memcpy(serial_framebuffer, startOfRemainingBytes, remainingBytesNb);
                    //unused bytes are zeroed
                    memset(serial_framebuffer + remainingBytesNb, 0, nbSerialBytes - remainingBytesNb);
                    serial_index = remainingBytesNb;
                }

                //TODO : for debug ease, erase checksum (it's appended at the end of the buffer)
                hdlc_framebuffer[hdlc_length] = 0;
                hdlc_framebuffer[hdlc_length+1] = 0;

                //treat the decoded frame
                LOG(INFO, "HDLC frame received size=" + String(hdlc_length) + " msg=[" + String(hdlc_framebuffer) + "]");
                //TODO decodage
            }
        } while(hdlc_length!=0); //there may be several frames, so as long as a frame is found, unpile buffer

        //clean dust bytes if an incomplete frame is holder the buffer beginning
        yahdlc_state_t serial_buf_state;
        yahdlc_get_state(&serial_buf_state);
        if( serial_buf_state.start_index < 0 ) //no start found, mean dust bytes at the begining of src buffer
        {
            memset(serial_framebuffer, 0, nbSerialBytes);
            serial_index = 0;
        }

        vTaskDelay(1);
    } //end while(2)
}

void TeleopThread::simpleSerialRun()
{
    while (2)
    {
        while (Serial.available())
        {
            auto cmd = Serial.read();
            LOG(INFO, "Command received : " + cmd);
            switch (cmd)
            {
            case 's': //Stats
            {
                g_ArdOs.displayStats();
                break;
            }

            case 'c': //configure
            {
                LOG(INFO, "configure match request received.");
                ROBOT.strategy.configureMatch(0, eColor::PREF);
                events[EVT_CONFIGURE].publish();
                break;
            }

            case 'g': //go
            {
                LOG(INFO, "start match request received.");
                events[EVT_START_MATCH].publish();
                break;
            }

            default:
            { //nothing done for now as pollution is generated by end of line characters
                LOG(DEBUG, "Teleop cmd " + String(cmd) + " unknown.");
                break;
            }
        }
    } //end while serial available
    vTaskDelay(1);
} //end while(2)
}

IEvent* TeleopThread::getEvent(eTeleopEvtId id)
{
    return &events[id];
}
