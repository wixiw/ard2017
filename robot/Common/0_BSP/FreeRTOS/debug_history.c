#include "debug_history.h"
#include "FreeRTOS_ARM.h"
#include <string.h>
#include "ArduinoCore/Arduino.h"

typedef struct
{
    uint32_t    timestamp;  //ms from the boot
    char*       eventName;  //a name to help the guy who debug to understand what he's doing at 4h in the morning
    int32_t    valueInt;   //a value that can be used to save something related to the event
    float       valueFloat; //a value that can be used to save something related to the event
} DebugEvent;

DebugEvent dh_table[NB_DEBUG_EVENTS];

uint16_t dh_index;      //critical var : to be modified only under critical section
uint8_t dh_overflows;   //critical var : to be modified only under critical section

void _dh_init()
{
    memset(dh_table, 0, sizeof(dh_table));
    for(int i = 0; i < NB_DEBUG_EVENTS; i++)
    {
        dh_table[i].eventName = malloc(DEBUG_EVENTS_NAME_LENGTH);
        memset(dh_table[i].eventName, 0, DEBUG_EVENTS_NAME_LENGTH);
    }
    dh_index = NB_DEBUG_EVENTS;
    dh_overflows = 0;
}

void _dh_publish_event(char const * const name, int32_t valueInt, float valueFloat)
{
    //enter critical section to manipulate indexes (prevent race condition)
    __disable_irq();

    uint32_t now = millis();

    //Manage debug history index overflow
    if( NB_DEBUG_EVENTS <= dh_index )
    {
        dh_index = 0;

        //Increment overflow counter (except if it also overflows)
        if(dh_overflows < 255)
            dh_overflows++;
    }
    else
        dh_index++;

    //Save event
    dh_table[dh_index].timestamp = now;
    strncpy(dh_table[dh_index].eventName, name, DEBUG_EVENTS_NAME_LENGTH);
    dh_table[dh_index].valueInt = valueInt;
    dh_table[dh_index].valueFloat = valueFloat;

    //quit critical section
    __enable_irq();
}


void _dh_publish_event_fromISR(char const * const name, int32_t valueInt, float valueFloat)
{
    //disable interrupts to prevent race condition
    __disable_irq();
    
    uint32_t now = millis();

    //Manage debug history index overflow
    if( NB_DEBUG_EVENTS <= dh_index )
    {
        dh_index = 0;

        //Increment overflow counter (except if it also overflows)
        if(dh_overflows < 255)
            dh_overflows++;
    }
    else
        dh_index++;

    //Save event
    dh_table[dh_index].timestamp = now;
    strncpy(dh_table[dh_index].eventName, name, DEBUG_EVENTS_NAME_LENGTH);
    dh_table[dh_index].valueInt = valueInt;
    dh_table[dh_index].valueFloat = valueFloat;

    //quit critical section
    __enable_irq();
}


