/*
 * UT_Actuators.cpp
 *
 * Created: 23/11/2016 23:58:55
 * Author : wix
 */ 

#include "RSP.h"
#include "common.h"

namespace ard{

AccelServo servo = AccelServo("servo", SERVO1);

class UT_AccelServo_Thread: public Thread
{
    public:
        UT_AccelServo_Thread():Thread("test", PRIO_MAX, STACK_UNIT_TEST){};

        void run() override
        {
            //init
            //bsp.serial0.println("UT_Actuators");
            //bsp.serial0.printStringln("Positioning servo to 0");
            servo.setAccMax(0);
            servo.setVmax(10000);
            servo.goTo(0);
            sleepMs(1000);

            //Move from 0 to 1000 at infinite speed
            servo.setAccMax(0);
            servo.setVmax(10000);
            servo.goTo(1000);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);
            //Move from 1000 to 0 at infinite speed
            servo.goTo(0);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);
//            //Move from 0 to 1000 at low speed
//            servo.setAccMax(0);
//            servo.setVmax(1);
//            servo.goTo(1000);
//            while(!servo.isTargetReached())
//            {
//                servo.update(PERIOD_ACTUATORS);
//                sleepMs(PERIOD_ACTUATORS);
//            }
//            //Move from 1000 to 0 at infinite speed
//            servo.goTo(0);
//            while(!servo.isTargetReached())
//            {
//                servo.update(PERIOD_ACTUATORS);
//                sleepMs(PERIOD_ACTUATORS);
//            }

            //Move from 0 to 1000 at nominal values
            servo.setAccMax(30);
            servo.setVmax(150);
            servo.goTo(1000);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);
            //Move from 1000 to 10 at infinite speed
            servo.goTo(10);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);

            //Move from 10 to 11 at nominal values
            servo.setAccMax(30);
            servo.setVmax(150);
            servo.goTo(11);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);
            //Move from 11 to 10 at nominal values
            servo.goTo(10);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);

            //Move from 0 to 1000 at low acc
            servo.setAccMax(1);
            servo.setVmax(150);
            servo.goTo(1000);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);
            //Move from 1000 to 0  at low acc
            servo.goTo(0);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);

            //Move from 0 to 1000 at high acc
            servo.setAccMax(10000);
            servo.setVmax(150);
            servo.goTo(1000);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);
            //Move from 1000 to 0 at infinite speed
            servo.goTo(0);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);

            //Move from 0 to 450 at nominal speed (a perfect bi-ramp)
            servo.setAccMax(30);
            servo.setVmax(150);
            servo.goTo(450);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);
            //Move from 450 to 0 at nominal speed (a perfect bi-ramp)
            servo.goTo(0);
            digitalWrite(LED_DUE_TX, LOW);
            while(!servo.isTargetReached())
            {
                servo.update(PERIOD_ACTUATORS);
                sleepMs(PERIOD_ACTUATORS);
            }
            digitalWrite(LED_DUE_TX, HIGH);
            sleepMs(1000);
            //bsp.serial0.printStringln(String("Target reached at (ms) : ") + String(millis()-start) + " (ms).");
        }

};

UT_AccelServo_Thread* threadAccelServo;

void UT_AccelServo(void)
{
    //bsp.serial0.setInterruptPriority(PRIORITY_IRQ_UART0);
    //bsp.serial0.start(/*baurate = */250000);

    threadAccelServo = new UT_AccelServo_Thread();
    pinMode(LED_DUE_TX, OUTPUT);

    ArdOs::init();
    ArdOs::start();
}

}
