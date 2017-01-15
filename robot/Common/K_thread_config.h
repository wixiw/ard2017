/*
 * K_thread_prio.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_K_THREAD_CONFIG_H_
#define ROBOTS_K_THREAD_CONFIG_H_

#include "0_BSP/FreeRTOS/FreeRTOS_ARM.h"

//threads priority lowest value, lowest priority
#define PRIO_TELEOP         (tskIDLE_PRIORITY + 1)
#define PRIO_LOG            (tskIDLE_PRIORITY + 2)
#define PRIO_STRATEGY       (tskIDLE_PRIORITY + 4)
#define PRIO_HMI            (tskIDLE_PRIORITY + 3)
#define PRIO_ACTUATORS      (tskIDLE_PRIORITY + 5)
#define PRIO_MAX            (tskIDLE_PRIORITY + 6)

//Take care config is in words, not bytes. 100 = 400o
//adjusted for having 200 words spare
#define STACK_TELEOP      400
#define STACK_LOG         500
#define STACK_HMI         300
#define STACK_STRATEGY    300
#define STACK_ACTUATORS   300

//Periods :
#define PERIOD_ACTUATORS         100 //ms
#define PERIOD_HMI                50 //ms
#define PERIOD_FAST_IT_US       1000 //us
#define PERIOD_VERY_FAST_IT_US    50 //us

#endif /* ROBOTS_K_THREAD_CONFIG_H_ */
