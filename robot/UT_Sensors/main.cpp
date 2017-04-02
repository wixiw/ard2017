/*
 * UT_Sensors.cpp
 *
 */ 

#include "BSP.hpp"
BSP bsp;

int main(void)
{
//    bool res = true;
//
//    //Init drivers
//    digitalWrite(LED_DUE_L, HIGH);
//    bsp.serial0.start(/*baurate = */115200);
//
//    //TODO test
//    //...
//    //...
//    res &= XX;
//
//    //No led on on success (only the GREEN on led)
//    if( res )
//    {
//        digitalWrite(LED_DUE_RX, HIGH);
//        digitalWrite(LED_DUE_TX, HIGH);
//        digitalWrite(LED_DUE_L, LOW);
//    }
//    //All led on on failure (all ORANGE)
//    else
//    {
//        digitalWrite(LED_DUE_RX, LOW);
//        digitalWrite(LED_DUE_TX, LOW);
//        digitalWrite(LED_DUE_L, HIGH);
//    }
//
//    while(1){};
}

extern String getExeVersion()
{
    return String("Version test : ") + __DATE__ + " " + __TIME__;
}
