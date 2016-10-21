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

//TODO value to be adjusted
#define STACK_TELEOP		 500
#define STACK_LOG		 500
#define STACK_HMI		 500

//Periods in ms
#define PERIOD_HMI		  50

#endif /* ROBOTS_K_THREAD_CONFIG_H_ */
