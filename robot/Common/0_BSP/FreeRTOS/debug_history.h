/*
 * debug_history.h
 *
 *  Created on: 26 févr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_0_BSP_DRIVERS_UTILITY_DEBUG_HISTORY_H_
#define ROBOT_COMMON_0_BSP_DRIVERS_UTILITY_DEBUG_HISTORY_H_

#include <stdint.h>

//Following functions provides a debug history feature to debug complex
//situation that cannot be debugged statically with a debugger
//
// An event is a timestamp + a name + 2 variables
//
//In order to be able to easily disable the feature, do not use functions
//directly, use the associated macro
//
//Note that interrupts are disabled during event record to prevent IT concurrency issues
//
// Usage example :
//
//  dh_init(); //in main() or anywhere in the init process
//  dh_publish_event("MyEvent", 12, 0);

//define DH_ENABLE to activate the feature in K_constants.h

// We may change this value to change the number of events in the queue
// The challenge is to get to big enougth value to record your problem
// and to fit to available RAM. Total RAM usage is in bytes :
// (12 + DEBUG_EVENTS_NAME_LENGTH)*NB_DEBUG_EVENTS
// It shall be less than 65k (as index is on uint16)
#define NB_DEBUG_EVENTS 500

// Same idea with the length of the text identifying the event
// Take care that the longer the string, the longer the critical
// section to store an event (which could cause issue with other interrupts)
#define DEBUG_EVENTS_NAME_LENGTH 10

#ifdef __cplusplus
extern "C"
{
#endif

//Initialize debug history, to be called before any action
#ifdef DH_ENABLE
    #define dh_init() _dh_init()
    //private declaration, do not use
    void _dh_init();
#else
    #define dh_init()
#endif

//Publish an event to the debug history, it will enter a critical section (both scheduler and interrupts are disabled)
// DO NOT USE FROM INTERRUPT, see dh_publish_event_fromISR
#ifdef DH_ENABLE
    #define dh_publish_event(name, valueInt, valueFloat) _dh_publish_event(name, valueInt, valueFloat)
    //private declaration, do not use
    void _dh_publish_event(char const * const name, int32_t valueInt, float valueFloat);
#else
    #define dh_publish_event()
#endif

//Publish an event to the debug history from interrupt, it will disable other interrupts
// DO NOT USE FROM THREAD, see dh_publish_event
#ifdef DH_ENABLE
    #define dh_publish_event_fromISR(name, valueInt, valueFloat) _dh_publish_event_fromISR(name, valueInt, valueFloat)
    //private declaration, do not use
    void _dh_publish_event_fromISR(char const * const name, int32_t valueInt, float valueFloat);
#else
    #define dh_publish_event_fromISR()
#endif

#ifdef __cplusplus
}
#endif


#endif /* ROBOT_COMMON_0_BSP_DRIVERS_UTILITY_DEBUG_HISTORY_H_ */
