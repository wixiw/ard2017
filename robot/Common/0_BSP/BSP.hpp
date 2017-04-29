/*
 * BSP.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef BSP_HPP_
#define BSP_HPP_

#ifndef __cplusplus
#error this file should not be included in a C project
#endif

#include "BSP.h"
#include "ArduinoCore/Arduino.h"
#include "CpuIo/ArdUART.h"
#include "CpuIo/DueTimer.h"
#include "CpuIo/GpioTools.h"
#include "CpuIo/SPI.h" //SPI shall be imported before SD
#include "CpuIo/SD.h"
#include "CpuIo/TWI_I2C.h"


typedef void(*IrqCB)(void);
extern IrqCB UART_Handler_CB;

class BSP
{
    public:
        //Initialize all pins mode. It's best to gather all pin init
        //at the same place to be sure everything is properly initialized
        //whatever if libs re-init them later, it's better to do it twice than to forget
        //and risk an HW prob
        BSP();


        //Rx0 / Tx0 serial driver
        ard::ArdUART serial0;
};

#endif /* BSP_HPP_ */
