#ifndef CONSTANTS_H
#define CONSTANTS_H

//Configure build
#define BUILD_REMOTE_CONTROL
//#define ARD_DEBUG

#define LOG_QUEUE_SIZE 20
#define LOG_MAX_SIZE 200

//Serial buffer set to 2 tiumes the HDLC value in hdlc.h
#define SERIAL_BUF_SIZE 3*1024

#define ENABLE_DETECTION 1

#define GAIN_ANALOG_TENSION_BATTERIE 0.024
#define DELAY_START_MS 1000
#define DELAY_RESTART_DETECTION_MS 500

//Baudrate : from 600 bits/s to 250kbits/s
#define SERIAL_BAUDRATE 250000

#define ACC_MAX 15000.0   //in step /s²
#define SPEED_RECAL 1800.0
#define SPEED_MAX_VIR 2000.0
#define SPEED_MAX 6000.0  //in step /s
#define STEP_BY_TURN 1600
#define WHEEL_PERIMETER 94.248 //mm
#define DURATION_MATCH_MS 90000


// DIMENSIONS : TODO verifier pour 2017
#define GAIN_STEP_MM 16.818
//#define GAIN_STEP_DEG 2000.0/218.0*1.75
#define VOIE 104.73
#define X_AR 34.0
#define Y_GAUCHE 60.0


#endif
