/*
 * K_thread_prio.h
 *
 *  Created on: 19 oct. 2016
 *      Author: wix
 */

#ifndef ROBOTS_K_THREAD_CONFIG_H_
#define ROBOTS_K_THREAD_CONFIG_H_

#include <stdint.h>

//A Stack size is given in words, not in bytes, hence a value of 100 stands for 400o
typedef uint16_t StackSize;

//Thread priority : the highest, the more prioritary
typedef uint16_t ThreadPriority;

//threads priority lowest value, lowest priority
static const ThreadPriority PRIO_UNSET          = 0xFFFF;
static const ThreadPriority PRIO_IDLE           = 0;
static const ThreadPriority PRIO_SDCARD_WRITER  = 1; //Really no critical
static const ThreadPriority PRIO_REMOTE_CTRL_RX = 2; //Remote control is for debug => lowest prio
static const ThreadPriority PRIO_REMOTE_CTRL_TX = 3; //Log is for debug but more important than remote control as we can have to read without requesting anything
static const ThreadPriority PRIO_HMI            = 4; //HMI is driving LEDS for feedback, it is usefull to see LED period is not stable, hence HMI has a low priority but just over "useless" stuff
static const ThreadPriority PRIO_STRATEGY       = 5; //There is no reason to send commands if the robot is overloaded, hence strategy is the lowest functional priority
static const ThreadPriority PRIO_ACTUATORS      = 6;
static const ThreadPriority PRIO_NAVIGATION     = 7; //Navigation has the highest rank because avoidance is the most critical task
#define                     PRIO_MAX            PRIO_NAVIGATION
#define                     PRIO_NB               8

//Take care config is in words, not bytes. 100 = 400o
//adjusted for having around 200 words spare in each task (except Idle which is really tight)
static const StackSize STACK_SDCARD_WRITER      = 500;
static const StackSize STACK_HMI                = 300;
static const StackSize STACK_REMOTE_CTRL_RX     = 700;
static const StackSize STACK_REMOTE_CTRL_TX     = 500;
static const StackSize STACK_STRATEGY           =1000; //This thread is subject to high stack changes in rougth debug/dev/no-sleep/no-eat condition, so we keep a lot of free space
static const StackSize STACK_ACTUATORS          =1000; //This thread is subject to high stack changes in rougth debug/dev/no-sleep/no-eat condition, so we keep a lot of free space
static const StackSize STACK_NAVIGATION         = 500;

//Periods :
#define PERIOD_ACTUATORS         100 //ms
#define PERIOD_HMI                50 //ms
#define PERIOD_NAVIGATION        100 //ms
#define PERIOD_FAST_IT_US       1000 //us
#define PERIOD_VERY_FAST_IT_US    50 //us

#endif /* ROBOTS_K_THREAD_CONFIG_H_ */
