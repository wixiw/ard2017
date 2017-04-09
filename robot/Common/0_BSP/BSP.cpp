#include "BSP.hpp"
#include "FreeRTOS/portable.h"

using namespace ard;

// IT handlers
IrqCB UART_Handler_CB = NULL;

void UART_Handler(void)
{
    if(UART_Handler_CB)
        UART_Handler_CB();
    else
        ASSERT(false);
}

/*
// IT handlers
void USART0_Handler(void)
{
    Serial1.IrqHandler();
}

void USART1_Handler(void)
{
    Serial2.IrqHandler();
}

void USART3_Handler(void)
{
    Serial3.IrqHandler();
}

USARTClass Serial1(USART0, USART0_IRQn, ID_USART0, &rx_buffer2, &tx_buffer2);
USARTClass Serial2(USART1, USART1_IRQn, ID_USART1, &rx_buffer3, &tx_buffer3);
USARTClass Serial3(USART3, USART3_IRQn, ID_USART3, &rx_buffer4, &tx_buffer4);
*/

BSP::BSP ():
        serial0(UART, ID_UART, SERIAL_BUF_SIZE /*RX bvuf size*/, SERIAL_BUF_SIZE /*TX bvuf size*/)
{   
    UART_Handler_CB = NULL;
    
    watchdogSetup();

    //#if defined(USBCON)
    //    USBDevice.attach();
    //#endif

    //pinMode(MOSFET1, OUTPUT);
    //pinMode(MOSFET2, OUTPUT);
    pinMode (SERVO1, 		        OUTPUT);
    pinMode (SERVO2, 		        OUTPUT);
    pinMode (SERVO3,		        OUTPUT);
    pinMode (SERVO4, 		        OUTPUT);
    pinMode (SERVO5, 		        OUTPUT);
    pinMode (SERVO6, 		        OUTPUT);
    pinMode (SERVO7, 		        OUTPUT);
    pinMode (SERVO8, 		        OUTPUT);
    pinMode (BUZZER, 		        OUTPUT);
    pinMode (LED_DUE_L, 		    OUTPUT);
    pinMode (BUTTON_START, 	        INPUT);
    pinMode (BUTTON_COLOR, 	        INPUT);
    pinMode (BUTTON_USER1, 	        INPUT);
    pinMode (BUTTON_USER2, 	        INPUT);
    pinMode (OMRON1, 		        INPUT_PULLUP);
    pinMode (OMRON2, 		        INPUT_PULLUP);
    pinMode (OMRON3, 		        INPUT_PULLUP);
    pinMode (OMRON4, 		        INPUT_PULLUP);
    pinMode (BORDURE_AVG, 		    INPUT);
    pinMode (BORDURE_AVD, 		    INPUT);
    pinMode (BORDURE_ARC,           INPUT);
    pinMode (SWITCH_ARM_L_OUT,      INPUT);
    pinMode (SWITCH_ARM_L_IN,       INPUT);
    pinMode (SWITCH_ARM_R_OUT,      INPUT);
    pinMode (SWITCH_ARM_R_IN,       INPUT);
    pinMode (OMRON_CYLINDER,        INPUT);
    pinMode (SWITCH_CYLINDER,       INPUT);
    pinMode (OMRON_SPARE,           INPUT);
    pinMode (SWITCH_LIFTER_UP,      INPUT);
    pinMode (SWITCH_LIFTER_DOWN,    INPUT);
    pinMode (CS_SDCARD, 		    OUTPUT);
    pinMode (PAPG_DIR, 		        OUTPUT);
    pinMode (PAPG_STEP, 		    OUTPUT);
    pinMode (PAPD_DIR, 		        OUTPUT);
    pinMode (PAPD_STEP, 		    OUTPUT);
    pinMode (DEBUG_1,     	        OUTPUT);
    pinMode (DEBUG_2,     	        OUTPUT);
    //pinMode (US_TRIG_1, 	OUTPUT);
    //pinMode (US_ECHO_1, 	INPUT);
    //pinMode (US_TRIG_2, 	OUTPUT);
    //pinMode (US_ECHO_2, 	INPUT);
    pinMode (PAP_ENABLE,		    OUTPUT);
    pinMode (LED1, 		            OUTPUT);
    pinMode (LED2, 		            OUTPUT);
    pinMode (LED3, 		            OUTPUT);
    pinMode (LED4, 		            OUTPUT);
    pinMode (LED_RGB_R, 		    OUTPUT);
    pinMode (LED_RGB_G, 		    OUTPUT);
    pinMode (LED_RGB_B, 		    OUTPUT);
    pinMode (LED_DUE_RX, 		    OUTPUT);
    pinMode (LED_DUE_TX, 		    OUTPUT);

    digitalWrite(LED_DUE_RX, HIGH);
    digitalWrite(LED_DUE_TX, HIGH);
    digitalWrite(LED_DUE_L, LOW);

}

void * operator new( size_t size )
{
    return pvPortMalloc( size );
}

void * operator new[]( size_t size )
{
    return pvPortMalloc(size);
}

void operator delete( void * ptr )
{
    vPortFree ( ptr );
}

void operator delete[]( void * ptr )
{
    vPortFree ( ptr );
}
