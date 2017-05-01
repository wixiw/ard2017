/*
 * UT_Actuators.cpp
 *
 * Created: 23/11/2016 23:58:55
 * Author : wix
 */ 

#include "RSP.h"
#include "common.h"

namespace ard{

AccelStepper stepperG = AccelStepper(AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR);

void UT_interrupt()
{
    stepperG.run();
}

class UT_Stepper_Thread: public Thread
{
    public:
        UT_Stepper_Thread():Thread("test", PRIO_MAX, STACK_UNIT_TEST){};

        void run() override
        {
            serial0.println("UT_Actuators");
            uint32_t start = millis();
            serial0.printStringln("Move " + String(millis()));
            stepperG.move(6000);
            digitalWrite(LED4, HIGH);
            Timer6.start(PERIOD_VERY_FAST_IT_US);

            while(stepperG.distanceToGo() != 0){};
            digitalWrite(LED4, LOW);
            serial0.printStringln("Stop, move duration (ms) : " + String(millis()-start));
        }

};

UT_Stepper_Thread* threadStepper;

void UT_Steppers(void)
{
    threadStepper = new UT_Stepper_Thread();

    serial0.setInterruptPriority(PRIORITY_IRQ_UART0);
    serial0.start(/*baurate = */SERIAL_BAUDRATE, SerialMode_8E1 | UART_MR_CHMODE_NORMAL);

    stepperG.setAcceleration(400);
    stepperG.setMaxSpeed(400);

    Timer6.attachInterrupt(UT_interrupt);
    pinMode(LED4, OUTPUT);

    ArdOs::init();
    ArdOs::start();

}


}
