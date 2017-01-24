/*
 * Robot2017.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "Robot2017.h"
#include "BSP.h"
#include "strategies/Strategies.h"

using namespace ard;

extern "C"
{
    extern void errorBlink(int n);
}

//check for Interrupt context !
//check for C++ demangling problems
void ardAssertImpl(bool condition, char const* file, unsigned int line, char const* text)
{
    if(!condition)
    {
        if( ArdOs::getState() == ArdOs::RUNNING && !Thread::interruptContext() )
        {
            LOG_ASSERT(String(file) +":" + line + String(text));
            ROBOT.dieMotherFucker();
        }
        else
        {
            errorBlink(3);
        }
    }
}

//singleton instanciation
Robot2017* Robot2017::instance = NULL;

//Use this interrupt to execute periodic stuff that shall run at a very high frequency (typically steppers)
//Obviously at such a frequence a Thread is too heavy as the context-switch duration would be higher than the period
void veryFast_interrupt()
{
    //  digitalWrite(DEBUG_1, 1); //uncomment to check period and delay with oscilloscope
    Robot2017::getInstance().nav.updateFromInterrupt();
    //  digitalWrite(DEBUG_1, 0); //uncomment to check period and delay with oscilloscope
}

//Use this interrupt to execute period stuff that shall run at a high frequency
//At this frequence a Thread is quite heavy as the context-switch duration would be roughtly equal to the period
void fast_interrupt()
{
    //  digitalWrite(DEBUG_2, 1); //uncomment to check period and delay with oscilloscope
    gpioToolsIsrCallback(PERIOD_FAST_IT_US);
    //  digitalWrite(DEBUG_2, 0); //uncomment to check period and delay with oscilloscope
}

Robot2017::Robot2017():
    actuators(),
    strategy(),
    nav(),
    hmi(50 /*ms*/),
    log(LogDispatcher::getInstance()),
#ifdef BUILD_REMOTE_CONTROL
    remoteControl(),
#endif
    fileLogger(LOG_QUEUE_SIZE)
{
    buildDate = String(__DATE__) + " " + __TIME__;
    strategy.registerStrategy("Alpha",          Strategy_Alpha);
    strategy.registerStrategy("Led Test",       Strategy_LedTest);
    strategy.registerStrategy("Button Test",    Strategy_ButtonTest);
    strategy.registerStrategy("Omron Test",     Strategy_OmronTest);
}

void Robot2017::bootOs()
{
    //Init drivers
    init_bsp();

    //Connect the log service
    Thread::setLogger(&log);
    log.addLogger(fileLogger);
#ifdef BUILD_REMOTE_CONTROL
    log.addLogger(remoteControl);
#endif

    //Map fast periodic functions to timers interrupts
    Timer6.attachInterrupt(veryFast_interrupt);
    Timer7.attachInterrupt(fast_interrupt);

    //Init debug serial link
    Serial.begin(/*baurate = */250000);

    //init all OS objects (including threads),
    //which should call all init() function
    //in the order of member declaration in the header file
    ArdOs::init();

    //heartbeat pour le debug pour verifier que le thread est vivant
    hmi.ledDue_Tx.slowBlink();

    //Start everything
    Timer6.start(PERIOD_VERY_FAST_IT_US);
    Timer7.start(PERIOD_FAST_IT_US);

    //Start all SW activities
    ArdOs::start();//this function never ends
}

void Robot2017::dieMotherFucker()
{
    //Ask the robot to stop moving and wait for it to be at rest
    nav.stopMoving();
    nav.wait();

    //stop every active SW activity, logs and remote controls are still active
    ROBOT.strategy.stopThread();
    ROBOT.actuators.stopThread();
    ROBOT.nav.stopThread();
    ROBOT.hmi.stopThread();
}

Robot2017& Robot2017::operator=(const Robot2017& p)
{
    *this = Robot2017(p);
    return *this;
}

IEvent* ard::Robot2017::getRemoteControlEvt(eRemoteControlEvtId id)
{
#ifdef BUILD_REMOTE_CONTROL
    return remoteControl.getEvent(id);
#else
    static Event<1> defaultEvent;
    return &defaultEvent;
#endif
}

Robot2017::Robot2017(const Robot2017& p)
: Robot2017()
{
}

bool Robot2017::isStartPlugged()
{
    return hmi.tirette.read();
}

IEvent* Robot2017::getStartInEvt()
{
    return hmi.tirette.getEvent(RISING_EDGE);
}

IEvent* Robot2017::getStartOutEvt()
{
    return hmi.tirette.getEvent(FALLING_EDGE);
}

bool Robot2017::isPreferedColor()
{
    return hmi.matchColor.read();
}

uint8_t Robot2017::getStrategyId()
{
    return (hmi.user1.read() << 1) + (hmi.user2.read());
}

void Robot2017::setRGBled(eRgb color, eLedState blink)
{
    hmi.ledRGB.set(color, blink);
}

void Robot2017::setLed(uint8_t led, eLedState blink)
{
    switch (led)
    {
    case LED1:
        hmi.led1.set(blink);
        break;

    case LED2:
        hmi.led2.set(blink);
        break;

    case LED3:
        hmi.led3.set(blink);
        break;

    case LED4:
        hmi.led4.set(blink);
        break;
    default:
        ASSERT_TEXT(false, "Unexpected value in setLed()");
        break;
    }
}
