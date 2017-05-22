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

// cf svn://109.10.110.62/ARD_svn/1_Eurobot_2017/Elec/IO Mapping _ ARD2017.xlsx

//0/1 : RX/TX 0
#define MOSFET1             2 //cylinder rotation
//#define MOSFET2           3
#define SERVO1              4 //Main Lifter
#define SERVO2              5 //Left arm
#define SERVO3              6 //Right arm
#define SERVO4              7 //Left arm wheel
#define SERVO5              8 //Right arm wheel
#define SERVO6              9 //Funny action
#define SERVO7	            10 //Rotateur
#define SERVO8              11 //Spare
#define BUZZER              12
#define LED_DUE_L           13 //used by ArdOs to signal OS errors
//14/15 : TX/RX 3 RESERVED FOR HOKUYO
//16/17 : TX/RX 2 RESERVED FOR WIFI
//18/19 : was TX/RX 1
#define BORDURE_ARC         18
#define DEBUG_1             19  //pin reserved for oscilloscope analysis
//20/21 : SDA/SCL I2C0 : color sensor
#define BUTTON_START        22
#define BUTTON_COLOR        23
#define BUTTON_USER1        24
#define BUTTON_USER2        25
#define OMRON2              26
#define OMRON3              27
#define OMRON4              28
#define BORDURE_AVG         29
#define BORDURE_AVD         30
#define OMRON1              31 //HW routing error : IN25 was forgetten, then mapped on 31 after production
#define CS_SDCARD           32 //not mapped to child card
#define SWITCH_CYLINDER     33
#define PAPG_DIR            34  // Gauche = PAP1
#define PAPG_STEP           35
#define PAPD_DIR            36  // Droite = PAP2
#define PAPD_STEP           37
#define OMRON_CYLINDER      38
#define OMRON_SPARE         39
#define SWITCH_ARM_L_IN     40
#define SWITCH_ARM_L_OUT    41
#define SWITCH_ARM_R_IN     42
#define SWITCH_ARM_R_OUT    43
#define PAP_ENABLE          44
#define SWITCH_LIFTER_DOWN  45
#define SWITCH_LIFTER_UP    46
#define LED1                47
#define LED2                48
#define LED3                49
#define LED4                50
#define DEBUG_2             LED4  //pin reserved for oscilloscope analysis
#define LED_RGB_R           51
#define LED_RGB_G           52
#define LED_RGB_B           53
#define LED_DUE_RX          72
#define LED_DUE_TX          73

#define ANA0_VBAT       A0

//Rise the debug1 pin to HIGH
#define DEBUG_SET_HIGH() PIO_Set(PIOA, PIO_PA10A_RXD0)
#define DEBUG_SET_LOW()  PIO_Clear(PIOA, PIO_PA10A_RXD0)

#endif /* BSP_H_ */
