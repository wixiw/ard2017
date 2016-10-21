#include "Arduino.h"
#include "pinout.h"
#include "constants.h"
#include <Servo.h>
#include <Scheduler.h>
#include "Button.h"
#include <AccelStepper.h>
#include "Navigation.h"
#include "RGB.h"
#include "Detection.h"
#include <math.h>
#include "Claws.h"
#include "Rod.h"
//#include "DueTimer.h"


Navigation nav = Navigation(stepperG,stepperD);
Detection detection = Detection(OMRON1, OMRON2, OMRON3, OMRON4);

color_t color = COLOR_UNDEF;

int started = 0;
int ended = 0;
int die_motherfucker = 0;
unsigned long dateStart = 0;

void setup() {
	pinMode(BUZZER, OUTPUT);
	pinMode(OMRON1, INPUT_PULLUP);
	pinMode(OMRON2, INPUT_PULLUP);
	pinMode(OMRON3, INPUT_PULLUP);
	pinMode(OMRON4, INPUT_PULLUP);

	stepperG->setMaxSpeed(V_MAX);
	stepperD->setMaxSpeed(V_MAX);
	stepperG->setAcceleration(ACC_MAX);
	stepperD->setAcceleration(ACC_MAX);

	rgb.set(BLUE,ON);
	rgb.run();
	// bip du pauvre :
	for (int i = 0; i < 100; i++)
	{
		digitalWrite(BUZZER, HIGH);
		delayMicroseconds(350);
		digitalWrite(BUZZER, LOW);
		delayMicroseconds(350);
	}
	rgb.off();rgb.run();

	rgb.set(RED,FAST);
}

void loop() {

	if (die_motherfucker)
	{
		yield();
		return;
	}

	if (btn_strat2.getRawState())
	{
		rgb.set(YELLOW,FAST);
		if (btn_color->getRawState() == 0)
		{
			color = GRN;
			rgb.set(GREEN,ON);
		}
		else
		{
			color = PRP;
			rgb.set(PURPLE,ON);
		}

		rod.setColor(color);
		test_claws();
		test_umbrella();
		test_release();
		yield();
		return;
	}

	if (ended)
	{
		funny_action();
		yield();
		return;
	}

	if (!started)
	{
		wait_start();
		digitalWrite(PAP_ENABLE, HIGH);
	}
	else
	{
		if (started && millis() - dateStart < DURATION_MATCH_MS)
		{
			strat_master();
			test_inputs();
		}
		else
		{
			digitalWrite(PAP_ENABLE, HIGH);
			ended = true;
			rgb.set(CYAN,FAST);
		}
		//		square();
		//		etalon();
	}

	//	test_buttons();

	yield();
}

void loop2()
//void motors()
{
	if (started)
	{
		if (!detection.getDetectionFront() && !detection.getDetectionRear())
		{
			nav.motorRun();
		}
	}
	yield();
}

// Appel de tous les runs
void loop3()
{
	if (!die_motherfucker)
	{
		detection.run();
	}

	yield();
}

void test_inputs(){
	digitalWrite(LED1 , digitalRead(OMRON1));
	digitalWrite(LED2 , digitalRead(OMRON2));
	digitalWrite(LED3 , digitalRead(OMRON3));
	digitalWrite(LED4 , digitalRead(OMRON4));
}

void test_buttons(){
	digitalWrite(LED1 , digitalRead(BUTTON_START));
	digitalWrite(LED2 , digitalRead(BUTTON_COLOR));
	digitalWrite(LED3 , digitalRead(BUTTON_STRAT1));
	digitalWrite(LED4 , digitalRead(BUTTON_STRAT2));
}

void etalon()
{
	digitalWrite(PAP_ENABLE, LOW);
	if (btn_start.newState() && btn_start.getCurrentState() == 0)
	{
		Serial.println("START");
		started = true;
	}

	if (started)
	{
		nav.turn(4*M_PI);
	}
}

void wait_start()
{
	static bool armed = false;

	detection.disableAll();

	if (btn_color->newState())
	{
		if (btn_color->getCurrentState() == 0)
		{
			color = GRN;
			rgb.set(GREEN,ON);
			rod.setColor(color);
		}
		else
		{
			color = PRP;
			rgb.set(PURPLE,ON);
			rod.setColor(color);
		}
		delay(1000);
	}


	if (!armed && btn_start.newState() && btn_start.getCurrentState() == 1)
	{
		armed = true;
	}

	if (armed)
	{
		if (color == GRN)
			rgb.set(GREEN,SLOW);
		else if (color == PRP)
			rgb.set(PURPLE,SLOW);
	}
	else
	{
		rgb.set(RED,FAST);
	}

	if (armed && btn_start.newState() && btn_start.getCurrentState() == 0)
	{
		started = true;
		dateStart = millis();
		if (color == GRN)
			rgb.set(GREEN,FAST);
		else if (color == PRP)
			rgb.set(PURPLE,FAST);
	}

	if (started)
	{
		// Initialize all components with the good color
		nav.setColor(color);
		rod.setColor(color);
	}
}

void funny_action(){
	delay(2000);
	srv_umbrella.write(UMBRELLA_RELEASED);
	rgb.set(WHITE,SLOW);
	die_motherfucker = true;
}

bool
strat_startup(){
	bool ret = false;
	static int state = 0;
	digitalWrite(PAP_ENABLE, LOW);

	// By default, enable avoidance
	// Is is disable after in the states
	detection.activateAll();

	switch (state)
	{
	case 0:
		detection.disableRear();
		state = 1;
		nav.startTraj();
		break;
	case 1:
		detection.disableRear();
		if (nav.straight(-20))
		{
			nav.setOdom(1500-X_AR, -100+Y_GAUCHE, -M_PI);
			nav.startTraj();
			state = 2;
		}
		break;
	case 2:
		detection.disableRear();
		if (nav.straight(250.0))
			state = 3;
		break;
	case 3:
		ret = true; // ready to go!
		break;
	default:;
	}

	return ret;
}





bool strat_die(){

	// By default, enable avoidance
	// Is is disable after in the states
	detection.activateAll();

	bool ret = false;
	switch (state_die){
	case ST_1:
		detection.disableRear();
		if (nav.go_s(0,-550))
			state_die = ST_2;
		break;
	case ST_2:
		detection.disableFront();
		if (nav.go_s(0,-550,MARCHE_AR))
			state_die = ST_3;
		break;
	case ST_3:
		detection.disableRear();
		if (detection.getDetectionFront())
		{
			delay(1000);
			state_die = ST_2;
		}
		if (nav.go_s(-750,-100))
			state_die = ST_1;
		break;

	default:;
	}

	return ret;
}

//
// __  __   _   ___ _____ ___ ___
//|  \/  | /_\ / __|_   _| __| _ \
//| |\/| |/ _ \\__ \ | | | _||   /
//|_|  |_/_/ \_\___/ |_| |___|_|_\
//

void strat_master(){
	digitalWrite(PAP_ENABLE, LOW);

	switch (state_master)
	{
	case ST_1:
		if (strat_startup())
			state_master = ST_1_1;
		break;
	case ST_1_1:
		if (strat_castle())
			state_master = ST_4;
		break;
	case ST_2:
		if (strat_start_shell(true))
			state_master = ST_7;
		break;
	case ST_3:
		if (strat_cabin())
			state_master = ST_4;
		break;
	case ST_4:
		if (strat_fish())
			state_master = ST_5;
		break;
	case ST_5:
		if (strat_fish())
			state_master = ST_6;
		break;
	case ST_6:
		if (strat_fish())
			if (btn_strat1.getRawState() == 0)
				state_master = ST_2; // seashell
			else
				state_master = ST_3; // CABINE
		break;
	case ST_7:
		// do nothing
	case ST_8:
		if (strat_die())
			state_master = ST_7;
		break;
	default:;
	}
}
