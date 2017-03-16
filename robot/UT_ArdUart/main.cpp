/*
 * UT_ArdUart.cpp
 *
 */ 

#include "BSP.h"
#include "ArdOs.h"
#include "variant.h"
#include "debug_history.h"

using namespace ard;

//Activate the Ard custom driver
#define NEW
//Activate the Arduino driver
//#define OLD

uint8_t trigger = 0;

void ledInterrupt()
{
    trigger = 1 - trigger;
    digitalWrite(LED_DUE_L, trigger);
}

#ifdef NEW
    BSP bsp;
#else
    class BspTest
    {
        public:
        //Initialize all pins mode. It's best to gather all pin init
        //at the same place to be sure everything is properly initialized
        //whatever if libs re-init them later, it's better to do it twice than to forget
        //and risk an HW prob
        BspTest():
        serial0(UART, UART_IRQn, ID_UART, &rx_buffer1, &tx_buffer1){};
    

        //Rx0 / Tx0 serial driver
        //extern ArdUART serial0;
        //extern UARTClass serial0;
        RingBuffer rx_buffer1;
        RingBuffer tx_buffer1;
        UARTClass serial0;
    };
    BspTest bsp;
#endif

void UT_ArdUART_UART_Handler()
{
    bsp.serial0.IrqHandler();
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
#ifdef NEW
        bsp.serial0.start(250000, SerialMode_8E1 | UART_MR_CHMODE_NORMAL);
#else
        bsp.serial0.begin(250000, SerialMode_8E1 | UART_MR_CHMODE_NORMAL);
        int res = -1;
#endif
        
        //TODO
        vTaskDelay(3000);

        while(state <= 3)
        {
            while(state <= 1)
            {
    #ifdef NEW
                bsp.serial0.read(&recvByte);
                ISerialDriver::eError err =  bsp.serial0.getError();
                if( err != ISerialDriver::ERR_NO_ERROR && firstByte == false)
                {
                    //bsp.serial0.write(uint8_t(0));
                    state = 4;
                    break;
                }
    #else
                res = -1;
                while(res == -1)
                    res = bsp.serial0.read();

                recvByte = res;
    #endif

                if( recvByte != 0)
                {
                    if( firstByte )
                        firstByte = false;
                        
                    state = 1;

                    //Le byte recu doit etre le precedent +1
                    if(recvByte != expectedByte)
                    {
                        //bsp.serial0.write(uint8_t(0));
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
                bsp.serial0.write(i+1);
            }
            bsp.serial0.flush();
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
#ifdef NEW
        bsp.serial0.start(250000, SerialMode_8E1 | UART_MR_CHMODE_REMOTE_LOOPBACK);
#else
        bsp.serial0.begin(250000, SerialMode_8E1 | UART_MR_CHMODE_REMOTE_LOOPBACK);
#endif
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
#ifdef NEW
        bsp.serial0.start(250000, SerialMode_8E1 | UART_MR_CHMODE_NORMAL);
#else
        bsp.serial0.begin(250000, SerialMode_8E1 | UART_MR_CHMODE_NORMAL);
        int res = -1;
#endif
 
        while(1)
        {
#ifdef NEW
            //block until a byte is available
            bsp.serial0.read(&recvByte);
#else
            //block until a byte is available
            res = -1;
            while(res == -1)
                res = bsp.serial0.read();
            recvByte = res;
#endif
            //loopback (except 0)
            if(recvByte)
                bsp.serial0.write(recvByte);
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
    
#ifdef NEW
    UART_Handler_CB = UT_ArdUART_UART_Handler;
    bsp.serial0.setInterruptPriority(PRIORITY_IRQ_UART0);
#endif
    ArdOs::init();
    ArdOs::start();
}

extern String getExeVersion()
{
    return String("Version test : ") + __DATE__ + " " + __TIME__;
}
