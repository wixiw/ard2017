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
#define PRIO_LOG			(tskIDLE_PRIORITY + 1)

//TODO a verifier
#define STACK_LOG		1024

#endif /* ROBOTS_K_THREAD_CONFIG_H_ */
