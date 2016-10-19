#ifndef CONSTANTS_H
#define CONSTANTS_H

#define ENABLE_DETECTION 1

#define GAIN_ANALOG_TENSION_BATTERIE 0.024
#define ANTIREBOND_MS 200
#define DELAY_START_MS 1000
#define DELAY_RESTART_DETECTION_MS 500


#define V_MAX 6000 //80000
#define ACC_MAX 15000.0
#define SPEED_RECAL 1800.0
#define SPEED_VIR 2000.0

#define DURATION_MATCH_MS 90000


// DIMENSIONS : TODO verifier pour 2017
#define GAIN_STEP_MM 8.409 // 16.818 // OK
//#define GAIN_STEP_DEG 2000.0/218.0*1.75
#define VOIE 104.73
#define X_AR 34.0
#define Y_GAUCHE 60.0

#define OSCILATOR 50 //ms

typedef enum{
	COLOR_UNDEF,
	PREF,
	SYM
} color_t;


#define MARCHE_AR -1
#endif
