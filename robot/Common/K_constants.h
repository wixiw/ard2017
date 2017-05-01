#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

//Serial buffer set to 3 times the HDLC value in hdlc.h
#define SERIAL_BUF_SIZE 3*1024

#ifndef DEG_TO_RAD
#define DEG_TO_RAD 0.017453292519943295769236907684886
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 57.295779513082320876798154814105
#endif

#define GAIN_ANALOG_TENSION_BATTERIE 0.024

    //Configure build
//#define ARD_DEBUG

#define LOG_QUEUE_SIZE 20       //number of logs that can be queued before being sent on the network
#define LOG_MAX_SIZE 200       //maximal log string length in characters

#define TABLE_TOP_Y         1000
#define TABLE_BORDER_X      1500

#define SERIAL_BAUDRATE 125000 //bit/s from 600 to 250k
#endif

