#include <Servo.h>

Servo servo;

void setup() {
	servo.attach(4);
}

void loop() {
	servo.write(160);
	delay(500 /*ms*/);
	servo.write(50);
	delay(500 /*ms*/);
}
