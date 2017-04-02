/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

	***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
	***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
	the FAQ page "My application does not run, what could be wrong?".  Have you
	defined configASSERT()?

	http://www.FreeRTOS.org/support - In return for receiving this top quality
	embedded software for free we request you assist our global community by
	participating in the support forum.

	http://www.FreeRTOS.org/training - Investing in training allows your team to
	be as productive as possible as early as possible.  Now you can receive
	FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
	Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*
 * The following #error directive is to remind users that a batch file must be
 * executed prior to this project being built.  Once it has been executed
 * remove the #error line below.
 */
//#error Ensure CreateProjectDirectoryStructure.bat has been executed before building.  See comment immediately above.

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include "K_thread_config.h"
#include "tc.h"

#define configUSE_PREEMPTION					1 //ARD : this is THE reason why we changed from a cooperative scheduler !
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1 //ARD : TODO could it help for -Onone boot issue ?
#define configUSE_IDLE_HOOK		        		0 //ARD : required to have the Arduino loop function (it's not sure it's required)
#define configUSE_MALLOC_FAILED_HOOK            1 //ARD : prevent from testing null pointers
#define configUSE_TICK_HOOK		        		0 //ARD : we don't use it
#define configCPU_CLOCK_HZ						F_CPU
#define configTICK_RATE_HZ						((TickType_t )1000) // ARD : it's quite high but usefull to get log timed properly. in case of high rate interrupts issues we could reduce this
#define configMAX_PRIORITIES					PRIO_NB //ARD : see K_thread_config.h
#define configMINIMAL_STACK_SIZE				20U 	//ARD : size of the idle task, take care it's in words, not in bytes : 100 means 400o
#define configMAX_TASK_NAME_LEN					(8+1) 	//ARD : +1 for the \0
#define configUSE_TRACE_FACILITY                1 // ARD : required for vTaskList to see stack info
#define configUSE_STATS_FORMATTING_FUNCTIONS    1 // ARD : required for vTaskList to see stack info
#define configUSE_16_BIT_TICKS		        	0 // ARD : there is no reason to use this as we are on a 32bits machine
#define configIDLE_SHOULD_YIELD                 0 // ARD : we don't use any task at idle priority
#define configUSE_TASK_NOTIFICATIONS            0 // ARD : it's a new freertos v9 feature we do not use yet
#define configUSE_MUTEXES			        	1 // ARD : we need it, to protect shared memories
#define configUSE_RECURSIVE_MUTEXES             1 // ARD : we need it
#define configUSE_COUNTING_SEMAPHORES           1 // ARD : we need it, to send signals between threads
#define configCHECK_FOR_STACK_OVERFLOW          2 // ARD : we need it, as the robot is always in debug condition we can't ensure stack are big enougth to be confident (2 is the strongest check), but time consuming during thread context switches
#define configQUEUE_REGISTRY_SIZE		        0 // ARD : we do not use OS aware debuggers so we don't need this
#define configUSE_QUEUE_SETS                    0 // ARD : it could be usefull, but not used yet
#define configUSE_TIME_SLICING                  0 // ARD : we use exclusive priorities only, time slicing is not required
#define configENABLE_BACKWARD_COMPATIBILITY     0 // ARD : we began with v8 so it's ok for us
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 0 // ARD : we do not use it
#define configSUPPORT_STATIC_ALLOCATION         1 // ARD : <3 <3 <3
#define configSUPPORT_DYNAMIC_ALLOCATION        0 // ARD : all static ! fuck off new()
#define configTOTAL_HEAP_SIZE					80000U
#define configGENERATE_RUN_TIME_STATS           1 // ARD : required to collect CPU stats
#define configUSE_APPLICATION_TASK_TAG          0 // ARD : TODO see what we can do with this, it could help to trace what's happening with context switches
/* Co-routine definitions : NOT USED*/
#define configUSE_CO_ROUTINES 			0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )
/* Software timer definitions : NOT USED */
#define configUSE_TIMERS				0
#define configTIMER_TASK_PRIORITY		( 2 )
#define configTIMER_QUEUE_LENGTH		0
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 0 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet	        	0
#define INCLUDE_uxTaskPriorityGet		        0
#define INCLUDE_vTaskDelete			        	0
#define INCLUDE_vTaskCleanUpResources       	0
#define INCLUDE_vTaskSuspend			        1
#define INCLUDE_vTaskDelayUntil		        	1
#define INCLUDE_vTaskDelay				        1
#define INCLUDE_eTaskGetState		        	0
#define INCLUDE_xTaskGetIdleTaskHandle          1 //ARD : required to drive the "CPU consuption" LED
#define INCLUDE_pcTaskGetTaskName               1 //ARD : required to log
#define INCLUDE_xTimerPendFunctionCall	        0 //ARD : SW timers not used
#define INCLUDE_uxTaskGetStackHighWaterMark     1 //ARD : required by getOsStats()

/**
 * Interrupts with priority between 0 and PRIORITY_IRQ_SYSCALL can be used if you
 * wish that the OS doesn't mask your interruption, this is
 * only possible if you do not use any freertos API call
 * (calls to BSP layer, like CMIS is obviously still possible)
 *
 * On the opposite side, if you wish to use the FreeRtos API (likely to use a semaphore
 * or signal), you need to have a priority between PRIORITY_IRQ_SYSCALL and PRIORITY_IRQ_KERNEL
 *
 * Take care, priorities are coded on the 4 highest bits, and NVIC_SetPriority is doing the job
 * of left shifting.
 */
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
    /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
    #define configPRIO_BITS            __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS            4        /* 15 priority levels */
#endif

//FreeRtos is using the raw priority byte value, hence we do the shifting
//that is normaly done by NVIC_SetPriority
//See K_thread_config.h for PRIORITY_IRQ_xxx defines
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    (PRIORITY_IRQ_SYSCALL << (8 - configPRIO_BITS))
#define configKERNEL_INTERRUPT_PRIORITY         (PRIORITY_IRQ_KERNEL << (8 - configPRIO_BITS))



/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
extern void ardAssertImpl(bool condition, char const* file, unsigned int line, char const* text);
#define configASSERT( x ) ardAssertImpl(x, __FILE__,__LINE__, "")

//------------------------------------------------------------------------------

//ARD :
/* Perform action when idle task start to run. */
void enterIdleCB();
#define traceTASK_SWITCHED_IN() \
  if( xTaskGetCurrentTaskHandle() == xTaskGetIdleTaskHandle() ) \
    enterIdleCB()

/* Perform action when the idle task stops running. */
void exitIdleCB();
#define traceTASK_SWITCHED_OUT() \
  if( xTaskGetCurrentTaskHandle() == xTaskGetIdleTaskHandle() ) \
    exitIdleCB()

//ARD : configure the timer used to compute CPU statistics
// see http://www.freertos.org/rtos-run-time-stats.html
extern void ardConfigureCpuStatTimer();
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() ardConfigureCpuStatTimer()

//ARD : read the timer used to compute CPU statistics
// see http://www.freertos.org/rtos-run-time-stats.html
#define portGET_RUN_TIME_COUNTER_VALUE() TC_ReadCV(TC0,0)

#define portRESET_TIMER_FOR_RUN_TIME_STATS()

#endif /* FREERTOS_CONFIG_H */
