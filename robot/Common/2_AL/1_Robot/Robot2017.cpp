/*
 * Robot2017.cpp
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#include "Robot2017.h"
#include "RSP.h"

using namespace ard;

//global var to let interrupts access to the robot instance
//child class or main shall set the variable.
Robot2017* Robot2017::instance = NULL;
extern void ardAssertImpl(bool condition, char const* file, unsigned int line, char const* text);
extern void veryFast_interrupt();
extern void fast_interrupt();

Robot2017::Robot2017():
    bsp(),
    log(LogDispatcher::getInstance()),
    fileLogger(LOG_QUEUE_SIZE),
    buildDate("unknown"),
    m_params(),
    hmi(TIMER_BUZZER),
    nav(hmi.buzzer),
    chrono(),
    stratInfo(),
    actuators(),
    lifecycle(nav, chrono, hmi),
    listener(NULL)
{
    ASSERT(instance == NULL);
    buildDate = String(__DATE__) + " " + __TIME__;
}

void Robot2017::init(Robot2017Listener* client)
{
    ASSERT_TEXT(instance == NULL,"You should not call init twice.");
    instance = this;

    ASSERT_TEXT(listener == NULL, "You should not replace the current client.");
    listener = client;

    ASSERT(instance);

    //Connect the log service
    Thread::setLogger(&log);
    log.addLogger(fileLogger);

    lifecycle.attach(this);

    //Map fast periodic functions to timers interrupts
    //GPIO_TIMER is used for CPU stats, see FreeRTOSConfig.h and FreeRTOS_ARM.c
    TIMER_NAV_STEPPER.attachInterrupt(veryFast_interrupt);
    TIMER_GPIO.attachInterrupt(fast_interrupt);

    //Configure interrupts priorities
    //not used with interrupts : TIMER_CPU
    TIMER_SERVO.setInterruptPriority            (PRIORITY_IRQ_SERVO);
    TIMER_NAV_STEPPER.setInterruptPriority      (PRIORITY_IRQ_STEPPERS);
    TIMER_GPIO.setInterruptPriority             (PRIORITY_IRQ_GPIO_FILTERS);
    TIMER_BUZZER.setInterruptPriority           (PRIORITY_IRQ_BUZZER);
    NVIC_SetPriority(WIRE_ISR_ID,                PRIORITY_IRQ_I2C0);
}

void Robot2017::boot()
{
    ASSERT(instance);

    //init all OS objects (including threads),
    //which should call all init() function
    //in the order of member declaration in the header file
    ArdOs::init();

    //heartbeat pour le debug pour verifier que le thread est vivant
    hmi.ledDue_Tx.slowBlink();
    hmi.led2.slowBlink();

    //Start everything
    TIMER_NAV_STEPPER.start(PERIOD_VERY_FAST_IT_US);
    TIMER_GPIO.start(PERIOD_FAST_IT_US);

    //Start all SW activities
    ArdOs::start();//this function never ends
}

YakardTimer& Robot2017::getFsmTimer()
{
    return lifecycle.fsmTimer;
}

bool Robot2017::isPen() const
{
    return 0 == strcmp(m_params.serialNumber(), "Pen");
}

bool Robot2017::isTration() const
{
    return 0 == strcmp(m_params.serialNumber(), "Tration");
}

void Robot2017::dieMotherFucker()
{
    LOG_INFO("Die mother fucker !!");

    //Ask the robot to stop moving and wait for it to be at rest
    nav.stopMoving();
    actuators.disableAll();

    //Play with LED for fun
    hmi.setRGBled(PURPLE, FAST_BLINK);
    hmi.setLed(LED1, OFF);
    hmi.setLed(LED2, OFF);
    hmi.setLed(LED3, OFF);
    hmi.setLed(LED4, OFF);
}

void Robot2017::bootUp()
{
    LOG_INFO(String("----------------- ") + getSerialNumber() + " -------------------");

    #ifdef ARD_DEBUG
    LOG_INFO(" --- DEBUG --- (see ARD_DEBUG in K_constants.h) ");
    #else
    LOG_INFO("Tips : In order to see debug logs, define ARD_DEBUG in ArdOs.h.");
    #endif

    //Trace binary version to prevent miss build error and usage error during the middle of the night.
    LOG_INFO("Version libArd : " + getVersion());
    LOG_INFO(getExeVersion());

    LOG_INFO(String("Robot is booted successfully, it took ") + millis() + " ms.");
    if( listener )
        listener->bootUp();

    //Display strategies to help choosing
    lifecycle.displayStrategies();
}

void Robot2017::colorChoosed(eColor color)
{
    if ( color == eColor_PREF )
    {
        hmi.setRGBled(YELLOW, ON);
        nav.setColor (eColor_PREF);
        stratInfo.setColor(eColor_PREF);
        actuators.setColor(eColor_PREF);
        LOG_INFO("User has selected PREF (Yellow) color.");
    }
    else if ( color == eColor_SYM )
    {
        hmi.setRGBled(BLUE, ON);
        nav.setColor (eColor_SYM);
        stratInfo.setColor(eColor_SYM);
        LOG_INFO("User has selected SYM (Blue) color.");
    }
    else
    {
        ASSERT_TEXT(false, "StrategyThread::configureMatch : color should not be unknown.");
    }
}

void Robot2017::matchEnded()
{
    dieMotherFucker();
}

void Robot2017::setConfig(apb_Configuration const& newConf)
{
    m_params.setConfig(newConf);
    nav.updateConf(&m_params);
}

char const * const Robot2017::getSerialNumber() const
{
    return m_params.serialNumber();
}
