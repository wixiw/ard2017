#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

//TODO a degager quand on passera pas une struct et plus des defines
#ifndef DEG_TO_RAD
#define DEG_TO_RAD 0.017453292519943295769236907684886
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 57.295779513082320876798154814105
#endif

//Mechanics/Electronics related constants
#define STEP_BY_TURN 1600
#define GAIN_ANALOG_TENSION_BATTERIE 0.024
#define X_AR 34.0
#define Y_GAUCHE 60.0
    //calibration params
#define LEFT_WHEEL_DIAMETER 60.000   //mm
#define RIGHT_WHEEL_DIAMETER 60.000  //mm
#define VOIE 104.73 //mm (distance between the 2 wheels contact points)

//Debug config
    //Configure build
#define BUILD_REMOTE_CONTROL
//#define ARD_DEBUG
#define LOG_QUEUE_SIZE 20
#define LOG_MAX_SIZE 200
    //Baudrate : from 600 bits/s to 250kbits/s
#define SERIAL_BAUDRATE 250000
    //Serial buffer set to 3 times the HDLC value in hdlc.h
#define SERIAL_BUF_SIZE 3*1024

//Navigation config
#define ENABLE_DETECTION 1
#define DELAY_RESTART_DETECTION_MS 500  //in ms
#define ACC_MAX  		1750.0          //in mm /s²
#define SPEED_RECAL 	 210.0          //in mm /s
#define SPEED_MAX_VIR 	 125.0          //in ° /s
#define SPEED_MAX 	 	 700.0          //in mm /s
#define DURATION_MATCH_MS 90000         //in ms

//Avoidance config
#define WAIT_FOR_OPP_MOVE 2000          //in ms

#ifdef __cplusplus //const keyword is not available for C
constexpr float GAIN_MM_2_STEPS_LEFT = - STEP_BY_TURN / (LEFT_WHEEL_DIAMETER * M_PI); // *-1 because left motor is inverted
constexpr float GAIN_MM_2_STEPS_RIGHT = STEP_BY_TURN / (RIGHT_WHEEL_DIAMETER * M_PI);
constexpr float GAIN_STEPS_2_MM_LEFT = - (LEFT_WHEEL_DIAMETER * M_PI) / STEP_BY_TURN; // *-1 because left motor is inverted
constexpr float GAIN_STEPS_2_MM_RIGHT = (RIGHT_WHEEL_DIAMETER * M_PI) /  STEP_BY_TURN;
constexpr float GAIN_DEG_2_MM_LEFT = DEG_TO_RAD * VOIE * GAIN_MM_2_STEPS_LEFT;
constexpr float GAIN_DEG_2_MM_RIGHT = DEG_TO_RAD * VOIE * GAIN_MM_2_STEPS_LEFT;
constexpr float GAIN_RAD_2_MM_LEFT = VOIE * GAIN_MM_2_STEPS_LEFT;
constexpr float GAIN_RAD_2_MM_RIGHT = VOIE * GAIN_MM_2_STEPS_RIGHT;
#endif //__cplusplus


#endif

