/*
 * UT_Helpers.cpp
 *
 * Created: 23/11/2016 23:58:55
 * Author : wix
 */ 

#include "BSP.hpp"

extern bool UT_buffer_tools();
using namespace ard;

ArdUART serial0(UART, ID_UART, SERIAL_BUF_SIZE /*RX bvuf size*/, SERIAL_BUF_SIZE /*TX bvuf size*/);

int main(void)
{
    bool res = true;

    digitalWrite(LED_DUE_L, HIGH);
    serial0.setInterruptPriority(0);
    serial0.start(/*baurate = */115200);



    //TODO test
    //...
    //...
    res &= UT_buffer_tools();

    //No led on on success (only the GREEN on led)
    if( res )
    {
        digitalWrite(LED_DUE_RX, HIGH);
        digitalWrite(LED_DUE_TX, HIGH);
        digitalWrite(LED_DUE_L, LOW);
    }
    //All led on on failure (all ORANGE)
    else
    {
        digitalWrite(LED_DUE_RX, LOW);
        digitalWrite(LED_DUE_TX, LOW);
        digitalWrite(LED_DUE_L, HIGH); 
    }

    while(1){};
}
