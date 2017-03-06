/*
 * UT_AccelStepper.cpp
 *
 * Created: 23/11/2016 23:58:55
 * Author : wix
 */ 


#include "sam.h"
#include "BSP.h"
#include "ArdOs.h"

using namespace ard;

AccelStepper stepperG = AccelStepper(AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR);
BSP bsp;

void UT_interrupt()
{
    stepperG.run();
}

void UT_AccelStepper_UART_Handler()
{
    bsp.serial0.IrqHandler();
}

class UT_AccelStepper: public Thread
{
    public:
        UT_AccelStepper():Thread("test", PRIO_MAX, STACK_UNIT_TEST){};
    
        void run() override
        {
            bsp.serial0.println("UT_AccelStepper");
            uint32_t start = millis();
            bsp.serial0.printStringln("Move " + String(millis()));
            stepperG.move(6000);      
            digitalWrite(LED4, HIGH);
            Timer6.start(PERIOD_VERY_FAST_IT_US);

            while(stepperG.distanceToGo() != 0){};
            digitalWrite(LED4, LOW);
            bsp.serial0.printStringln("Stop, move duration (ms) : " + String(millis()-start));
        }    
    
};

UT_AccelStepper thread;

int main(void)
{
    UART_Handler_CB = UT_AccelStepper_UART_Handler;
    bsp.serial0.setInterruptPriority(PRIORITY_IRQ_UART0);
    bsp.serial0.start(/*baurate = */250000);

    stepperG.setAcceleration(ACC_MAX);
    stepperG.setMaxSpeed(SPEED_MAX);

    Timer6.attachInterrupt(UT_interrupt);
    pinMode(LED4, OUTPUT);

    ArdOs::init();
    ArdOs::start();

}

extern String getExeVersion()
{
    return String("Version test : ") + __DATE__ + " " + __TIME__;
}
