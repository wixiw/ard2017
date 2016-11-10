/*
 * K_thread_prio.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_K_THREAD_CONFIG_H_
#define ROBOTS_K_THREAD_CONFIG_H_

#include <FreeRTOS_ARM.h>

//threads priority lowest value, lowest priority
#define PRIO_TELEOP			(tskIDLE_PRIORITY + 1)
#define PRIO_LOG			(tskIDLE_PRIORITY + 2)
#define PRIO_HMI			(tskIDLE_PRIORITY + 3)
#define PRIO_STRATEGY			(tskIDLE_PRIORITY + 4)
#define PRIO_ACTUATORS			(tskIDLE_PRIORITY + 5)
#define PRIO_RESERVED_OS		(tskIDLE_PRIORITY + 6)

//TODO value to be adjusted
#define STACK_TELEOP		 500
#define STACK_LOG		 500
#define STACK_HMI		 500
#define STACK_STRATEGY		 500
#define STACK_ACTUATORS		 500

//Periods in ms
#define PERIOD_ACTUATORS	  10
#define PERIOD_HMI		  50

#endif /* ROBOTS_K_THREAD_CONFIG_H_ */
