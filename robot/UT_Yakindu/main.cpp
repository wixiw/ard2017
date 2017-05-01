/*
 * UT_Yakindu.cpp
 *
 */ 

#include "AL.h"
#include "BlinkLED.h"
#include "BlinkLEDWrapper.h"
using namespace ard;

ArdUART serial0(UART, ID_UART, SERIAL_BUF_SIZE /*RX bvuf size*/, SERIAL_BUF_SIZE /*TX bvuf size*/);

int main(void)
{
    bool res = true;

    //Init drivers
    digitalWrite(LED_DUE_L, HIGH);
    serial0.setInterruptPriority(0);
    serial0.start(/*baurate = */SERIAL_BAUDRATE);

    BlinkLED sm;
    YakardTimer yakTim;
    BlinkLEDWrapper smWrapper;
    sm.setTimer(&yakTim);
    sm.setSCI_BSP_OCB(&smWrapper);

    sm.init();
    sm.enter();
    res &= sm.isStateActive(BlinkLED::main_region_Init);
    sm.runCycle();
    res &= sm.isStateActive(BlinkLED::main_region_Init);
    sm.raise_start();
    res &= sm.isStateActive(BlinkLED::main_region_Init);
    sm.runCycle();
    res &= sm.isStateActive(BlinkLED::main_region_LEDOff);

    unsigned long current_millies = millis();
    unsigned long last_millies = millis();
    unsigned long period = 10;
    while(1)
    {
        current_millies = millis();
        if( period < current_millies - last_millies)
        {
            yakTim.run(current_millies - last_millies);
            sm.runCycle();
            last_millies = current_millies;
        }
    }

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

