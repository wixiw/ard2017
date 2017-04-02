/*
 * BSP.h
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#ifndef BSP_H_
#define BSP_H_

#include "ArduinoCore/Arduino.h"
#include "ArduinoCore/Stream.h"
#include "CMSIS/CMSIS/Include/core_cm3.h"
#include "CMSIS/Device/ATMEL/sam.h"

#include "variant.h"
#include "K_constants.h"

//0/1 : RX/TX 0
//#define MOSFET1         2
//#define MOSFET2         3
#define SERVO1          4
#define SERVO2          5
#define SERVO3          6
#define SERVO4          7
#define SERVO5          8
#define SERVO6          9
#define SERVO7          10
#define SERVO8          11
#define BUZZER          12
#define LED_DUE_L       13
//14/15 : TX/RX 3
//16/17 : TX/RX 2
//18/19 : TX/RX 1
//20/21 : SDA/SCL
#define BUTTON_START    22
#define BUTTON_COLOR    23
#define BUTTON_USER1    24
#define BUTTON_USER2    25
#define OMRON1          31 //HW routing error : IN25 was forgetten, then mapped on 31 after production
#define OMRON2          26
#define OMRON3          27
#define OMRON4          28
#define BORDURE_G       29
#define BORDURE_D       30
//Note : See above for  31 (Omron1)
#define CS_SDCARD	32  //not mapped to child card
//33 : free IO, not mapped on childboard
#define PAPG_DIR        34  // Gauche = PAP1
#define PAPG_STEP       35
#define PAPD_DIR        36  // Droite = PAP2
#define PAPD_STEP       37
#define DEBUG_1         38  //pin reserved for oscilloscope analysis
#define DEBUG_2         39  //pin reserved for oscilloscope analysis
//38/39 : free IO, not mapped on childboard
//#define US_TRIG_1       40
//#define US_ECHO_1       41
//#define US_TRIG_2       42
//#define US_ECHO_2       43
#define PAP_ENABLE      44
//45/46 : free IO, not mapped on childboard
#define LED1            47
#define LED2            48
#define LED3            49
#define LED4            50
#define LED_RGB_R       51
#define LED_RGB_G       52
#define LED_RGB_B       53
#define LED_DUE_RX	72
#define LED_DUE_TX	73

#define ANA0_VBAT       A0

#endif /* BSP_H_ */
