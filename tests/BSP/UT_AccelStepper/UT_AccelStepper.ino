// -*- mode: C++ -*-
//
// Make a single stepper perform random changes in speed, position and acceleration
//
// Copyright (C) 2009 Mike McCauley
// $Id: Random.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

#define PAPG_DIR        34  // Gauche = PAP1
#define PAPG_STEP       35
#define PAPD_DIR        36  // Droite = PAP2
#define PAPD_STEP       37

// Define a stepper and the pins it will use
AccelStepper stepperG(AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR);
AccelStepper stepperD(AccelStepper::DRIVER, PAPD_STEP, PAPD_DIR);

void setup() {
}

void loop() {
	if (stepperG.distanceToGo() == 0 && stepperD.distanceToGo() == 0) {
		// Random change to speed, position and acceleration
		// Make sure we dont get 0 speed or accelerations
		delay(1000);
		stepperG.moveTo(rand() % 200);
		stepperG.setMaxSpeed((rand() % 200) + 1);
		stepperG.setAcceleration((rand() % 200) + 1);
		stepperD.moveTo(rand() % 200);
		stepperD.setMaxSpeed((rand() % 200) + 1);
		stepperD.setAcceleration((rand() % 200) + 1);
	}
	stepperG.run();
	stepperD.run();
}
