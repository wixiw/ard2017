/*
 * UT_Helpers.cpp
 *
 * Created: 23/11/2016 23:58:55
 * Author : wix
 */ 

#include "BSP.hpp"

extern bool UT_buffer_tools();
extern bool UT_graph();
using namespace ard;

int main(void)
{
    bool res = true;

    digitalWrite(LED_DUE_L, HIGH);

    res &= UT_buffer_tools();
    res &= UT_graph();

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
