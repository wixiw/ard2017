/*
 * Robot2017.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "Robot2017.h"
#include "BSP.h"
#include "TestStrategies.h"
#include "StrategyAlpha.h"

using namespace ard;

//singleton instanciation
Robot2017 Robot2017::instance = Robot2017();

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

Robot2017::Robot2017()
        : actuators(), strategy(), claws(), nav(), hmi(50 /*ms*/), log(LogThread::getInstance())
#ifdef BUILD_TELEOP
                , teleop()
#endif
{
    actuators.addMiniThread(&nav);
    actuators.addMiniThread(&claws);
    strategy.registerStrategy("Alpha", Strategy_Alpha);
    strategy.registerStrategy("Led Test", Strategy_LedTest);
    strategy.registerStrategy("Button Test", Strategy_ButtonTest);
    strategy.registerStrategy("Omron Test", Strategy_OmronTest);
}

void Robot2017::boot()
{
    init_bsp();

    //Map fast periodic functions to timers interrupts
    Timer6.attachInterrupt(veryFast_interrupt);
    Timer7.attachInterrupt(fast_interrupt);

    //Init debug serial link
    Serial.begin(/*baurate = */250000);
    g_ArdOs.dprintln(String("[libARD] ") + "Build : " + __DATE__ + " " + __TIME__ );

    //Threads init
    hmi.init();
    hmi.ledDue_Tx.slowBlink(); //pour le debug pour verifier que le thread est vivant

    log.init();
#ifdef BUILD_TELEOP
    teleop.init();
#endif
    actuators.init();
    strategy.init();

    //Start everything
    Timer6.start(PERIOD_VERY_FAST_IT_US);
    Timer7.start(PERIOD_FAST_IT_US);

    //init OS
    g_ArdOs.init(); //this function never ends
}

void Robot2017::dieMotherFucker()
{
    nav.stop();
    nav.wait();
    g_ArdOs.die();
}

Robot2017& Robot2017::operator=(const Robot2017& p)
{
    *this = Robot2017(p);
    return *this;
}

IEvent* ard::Robot2017::getTeleopEvt(eTeleopEvtId id)
{
#ifdef BUILD_TELEOP
    return teleop.getEvent(id);
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
    return hmi.start.read();
}

IEvent* Robot2017::getStartInEvt()
{
    return hmi.start.getEvent(RISING_EDGE);
}

IEvent* Robot2017::getStartOutEvt()
{
    return hmi.start.getEvent(FALLING_EDGE);
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
        ardAssert(false, "Unexpected value in setLed()");
        break;
    }
}
