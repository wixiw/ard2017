/**
 * \file
 * \brief FreeRTOS for Teensy 3.x and Due
 */
#ifndef FreeRTOS_ARM_h
#define FreeRTOS_ARM_h

#ifndef __arm__
#error ARM Due or Teensy 3.x required
#else  // __arm__
//------------------------------------------------------------------------------
/** FreeRTOS_ARM version YYYYMMDD */
#define FREE_RTOS_ARM_VERSION 20151113
//------------------------------------------------------------------------------
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "portmacro.h"
#endif  // __arm__
#endif  // FreeRTOS_ARM_h