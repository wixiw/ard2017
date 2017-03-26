#include "RSP.h"

using namespace ard;

#define TIMER_BUZZER        Timer4
//HmiThread hmiThread(50 /*ms*/, TIMER_BUZZER);
Buzzer buzzer = Buzzer(TIMER_BUZZER, BUZZER, 10);

class UT_LEDs_Thread: public Thread
{
public:
    UT_LEDs_Thread()
            : Thread("test", tskIDLE_PRIORITY + 1, STACK_UNIT_TEST)
    {
    }
    ;

    void run() override
    {
        buzzer.init(true);
        buzzer.bip(3);
        while (1)
        {
//            hmiThread.led1.on();
//            digitalWrite(LED_DUE_RX, 1);
//            sleepMs(500);
//            hmiThread.led1.off();
            digitalWrite(LED_DUE_RX, 0);
            //sleepMs(500);
            digitalWrite(LED_DUE_RX, 1);
            //sleepMs(500);
              
        }
    }
};

UT_LEDs_Thread threadLeds;

void UT_LEDs()
{
    TIMER_BUZZER.setInterruptPriority           (PRIORITY_IRQ_BUZZER);
    ArdOs::init();
    ArdOs::start();
}
