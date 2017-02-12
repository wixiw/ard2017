/*
 * UT_AccelStepper.cpp
 *
 * Created: 23/11/2016 23:58:55
 * Author : wix
 */ 


#include "sam.h"
#include "BSP.h"

AccelStepper stepperG = AccelStepper(AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR);

void UT_interrupt()
{
    stepperG.run();
}

int main(void)
{
    //Init drivers
    init_bsp();
    Serial.begin(/*baurate = */115200);

    stepperG.setAcceleration(ACC_MAX);
    stepperG.setMaxSpeed(SPEED_MAX);
    stepperG.move(6000);

    Timer6.attachInterrupt(UT_interrupt);
    pinMode(LED4, OUTPUT);

    Serial.println("Move " + String(millis()));
    digitalWrite(LED4, HIGH);
    Timer6.start(PERIOD_VERY_FAST_IT_US);



    while(stepperG.distanceToGo() != 0){};
    digitalWrite(LED4, LOW);
    Serial.println("Stop " + String(millis()));

    while(1){};
}

extern String getExeVersion()
{
    return String("Version robotA : ") + __DATE__ + " " + __TIME__;
}
