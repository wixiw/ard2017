/*
 * UT_ArdUart.cpp
 *
 */ 

#include "BSP.hpp"
#include "ArdOs.h"
#include "variant.h"
#include "debug_history.h"
#include "RobotParameters.h"
#include "ArdHdlc.h"

using namespace ard;

uint8_t trigger = 0;
BSP bsp;
ArdUART serial0(UART, ID_UART, SERIAL_BUF_SIZE /*RX bvuf size*/, SERIAL_BUF_SIZE /*TX bvuf size*/);

void ledInterrupt()
{
    trigger = 1 - trigger;
    digitalWrite(LED_DUE_L, trigger);
}

void UART_Handler()
{
    serial0.IrqHandler();
}



/**
 * In order to run this test you are supposed to send bytes continously in burst
 * increasing by 1 (starting at 1). The test loops until bytes received are ok
 * And send back the received byte. You are supposed to check the good reception
 *
 * It exits when byte incrementation is not ok and send a 0
 */

//------------------------------------------------------------------------------
// high priority thread to set pin low
uint8_t recvByte = 0;
uint8_t expectedByte = 1;
uint8_t state = 0;
bool firstByte = true;
        
class UartUnpileThread: public Thread
{
public:
    UartUnpileThread():Thread("UART", tskIDLE_PRIORITY + 1, 3000){};

    void run() override
    {
        serial0.start(SERIAL_BAUDRATE, SerialMode_8E1 | UART_MR_CHMODE_NORMAL);
        
        //TODO
        vTaskDelay(3000);

        while(state <= 3)
        {
            while(state <= 1)
            {
                serial0.read(&recvByte);
                ISerialDriver::eError err =  serial0.getError();
                if( err != ISerialDriver::ERR_NO_ERROR && firstByte == false)
                {
                    //serial0.write(uint8_t(0));
                    state = 4;
                    break;
                }

                if( recvByte != 0)
                {
                    if( firstByte )
                        firstByte = false;
                        
                    state = 1;

                    //Le byte recu doit etre le precedent +1
                    if(recvByte != expectedByte)
                    {
                        //serial0.write(uint8_t(0));
                        state = 4;
                        break;
                    }
                    else
                    {
                        //end of burst reception
                        expectedByte++;
                        if(expectedByte == 0)
                        {
                            expectedByte = 1;
                            state = 2;
                        }                            
                    }

                }
                else
                {
                    //0 received
                }
            }

            if(state == 4)
            {
                break;
            }

            state = 3;
            for(int i = 0 ; i < 255 ; i++)
            {
                serial0.write(i+1);
            }
            serial0.flush();
            state = 0;
        }

        digitalWrite(LED_DUE_RX, LOW);
        digitalWrite(LED_DUE_TX, LOW);
        digitalWrite(LED_DUE_L, HIGH); 
    }
};


/************************************************************************/
/* This test configure the serial driver to electronically 
   loopback received bytes, it is particularly usefull to test the serial line robustness 
   to electronical perturbations
*/
/************************************************************************/
class RemoteLoopbackThread: public Thread
{
public:
    RemoteLoopbackThread():Thread("UART", tskIDLE_PRIORITY + 1, STACK_UNIT_TEST){};

    void run() override
    {  
        serial0.start(SERIAL_BAUDRATE, SerialMode_8E1 | UART_MR_CHMODE_REMOTE_LOOPBACK);
    }
};

/************************************************************************/
/* This test returns any received byte to the sender 
*/
/************************************************************************/
class SwLoopbackThread: public Thread
{
public:
    SwLoopbackThread():Thread("UART", tskIDLE_PRIORITY + 1, STACK_UNIT_TEST){};

    void run() override
    {  
        serial0.start(SERIAL_BAUDRATE, SerialMode_8E1 | UART_MR_CHMODE_NORMAL);
 
        while(1)
        {
            //block until a byte is available
            serial0.read(&recvByte);
            //loopback (except 0)
            if(recvByte)
                serial0.write(recvByte);
        }            
    }
};


UartUnpileThread thread;
//RemoteLoopbackThread thread;
//SwLoopbackThread thread;

int main(void)
{
    digitalWrite(LED_DUE_RX, HIGH);
    digitalWrite(LED_DUE_TX, HIGH);
    digitalWrite(LED_DUE_L, LOW);
    Timer6.setInterruptPriority(PRIORITY_IRQ_SYSCALL - 1);
    Timer6.attachInterrupt(ledInterrupt);
    Timer6.start(500000);
    dh_init();
    
    serial0.setInterruptPriority(PRIORITY_IRQ_UART0);
    ArdOs::init();
    ArdOs::start();
}
