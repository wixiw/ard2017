/*
 * K_thread_prio.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_K_THREAD_CONFIG_H_
#define ROBOTS_K_THREAD_CONFIG_H_

//A Stack size is given in words, not in bytes, hence a value of 100 stands for 400o
typedef uint16_t StackSize;

//Thread priority : the highest, the more prioritary
typedef uint16_t ThreadPriority;

//threads priority lowest value, lowest priority
static const ThreadPriority PRIO_UNSET          = 0xFFFF;
static const ThreadPriority PRIO_IDLE           = 0;
static const ThreadPriority PRIO_BOOT           = 1; //This thread is used to do initialization with OS running, it must be the lowest priority task sothat started/configured tasks can run  after changes
static const ThreadPriority PRIO_HMI            = 2; //HMI is driving LEDS for feedback, it is usefull to see LED period is not stable, hence HMI has a low priority
static const ThreadPriority PRIO_REMOTE_CTRL    = 3; //Teleoperation is for debug => lowest prio
static const ThreadPriority PRIO_LOG            = 4; //Log is for debug but more important than teleop as we can have to read without requesting anything
static const ThreadPriority PRIO_STRATEGY       = 5; //There is no reason to send commands if the robot is overloaded, hence strategy is the lowest functional priority
static const ThreadPriority PRIO_ACTUATORS      = 6;
static const ThreadPriority PRIO_NAVIGATION     = 7; //Navigation has the highest rank because avoidance is the most critical task
#define                     PRIO_MAX            PRIO_NAVIGATION
#define                     PRIO_NB               8

//Take care config is in words, not bytes. 100 = 400o
//adjusted for having around 200 words spare in each task (except Idle which is really tight)
static const StackSize STACK_BOOT               = 1000;
static const StackSize STACK_HMI                = 1000;
static const StackSize STACK_REMOTE_CTRL        = 1000;
static const StackSize STACK_LOG                = 1000;
static const StackSize STACK_STRATEGY           = 1000;
static const StackSize STACK_ACTUATORS          = 1000;
static const StackSize STACK_NAVIGATION         = 1000;

//Periods :
#define PERIOD_ACTUATORS         100 //ms
#define PERIOD_HMI                50 //ms
#define PERIOD_NAVIGATION        100 //ms
#define PERIOD_FAST_IT_US       1000 //us
#define PERIOD_VERY_FAST_IT_US    50 //us

#endif /* ROBOTS_K_THREAD_CONFIG_H_ */
