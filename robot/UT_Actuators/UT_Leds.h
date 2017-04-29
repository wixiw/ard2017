

#include "RSP.h"
#include "common.h"

namespace ard
{

class UT_LEDs_Thread: public Thread
{
public:
    UT_LEDs_Thread()
            : Thread("test", PRIO_MAX, STACK_UNIT_TEST)
    {
    }
    ;

    void run() override
    {
        while (1)
        {
            digitalWrite(LED_DUE_TX, 0);
            digitalWrite(LED4, 0);
            sleepMs(500);
            digitalWrite(LED_DUE_TX, 1);
            digitalWrite(LED4, 1);
            sleepMs(500);
              
        }
    }
};

UT_LEDs_Thread* threadLeds;


void UT_LEDs()
{
    bsp = new BSP();
    threadLeds = new UT_LEDs_Thread();
    ArdOs::init();
    ArdOs::start();
}

}
