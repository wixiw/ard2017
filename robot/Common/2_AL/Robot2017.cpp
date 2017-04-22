/*
 * Robot2017.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "Robot2017.h"

//Timer 0 reserved for ARD cpu stats, instanciated for IRQ priority config
#define TIMER_CPU           Timer0
//Timer 1 reserved for the Servo lib, instanciated for IRQ priority config
#define TIMER_SERVO         Timer5

//Timer available for Duetimer lib
#define TIMER_NAV_STEPPER   Timer2
#define TIMER_GPIO          Timer3
#define TIMER_BUZZER        Timer4

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
            Robot2017::getInstance().dieMotherFucker();
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
    //  DEBUG_SET_HIGH(); //uncomment to check period and delay with oscilloscope
    Robot2017::getInstance().nav.updateFromInterrupt();
    //  DEBUG_SET_LOW(); //uncomment to check period and delay with oscilloscope
}

//Use this interrupt to execute period stuff that shall run at a high frequency
//At this frequence a Thread is quite heavy as the context-switch duration would be roughtly equal to the period
void fast_interrupt()
{
//    DEBUG_SET_HIGH(); //uncomment to check period and delay with oscilloscope
    gpioToolsIsrCallback(PERIOD_FAST_IT_US);
//    DEBUG_SET_LOW(); //uncomment to check period and delay with oscilloscope
}

void UART_Handler(void)
{
    //DEBUG_SET_HIGH(); //uncomment to check period and delay with oscilloscope

    static ArdUART* uart = NULL;
    if( uart == NULL )
        uart = &Robot2017::getInstance().bsp.serial0;
    uart->IrqHandler();

    //DEBUG_SET_LOW(); //uncomment to check period and delay with oscilloscope : default empty duration (2 io write + 1 function call) is 750ns
}

Robot2017::Robot2017():
    bsp(),
#ifdef BUILD_STRATEGY
    actuators(),
    lifecycle(this),
    fsmTimer(),
    strategy(),
#endif
    nav(),
    chrono(),
    hmi(TIMER_BUZZER),
    log(LogDispatcher::getInstance()),
#ifdef BUILD_REMOTE_CONTROL
    remoteControl(this, bsp.serial0),
#endif
    fileLogger(LOG_QUEUE_SIZE),
    buildDate("unknown"),
    m_params()
{
    buildDate = String(__DATE__) + " " + __TIME__;
}

void Robot2017::bootOs()
{
    nav.updateConf(&m_params);

    //Connect the log service
    Thread::setLogger(&log);
    log.addLogger(fileLogger);
#ifdef BUILD_REMOTE_CONTROL
    log.addLogger(remoteControl);
#endif

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
    bsp.serial0.setInterruptPriority            (PRIORITY_IRQ_UART0);
    NVIC_SetPriority(WIRE_ISR_ID,                PRIORITY_IRQ_I2C0);

    //init all OS objects (including threads),
    //which should call all init() function
    //in the order of member declaration in the header file
    ArdOs::init();

    //heartbeat pour le debug pour verifier que le thread est vivant
    hmi.ledDue_Tx.slowBlink();
    hmi.led2.slowBlink();

    //Start everything
    bsp.serial0.start(SERIAL_BAUDRATE, SerialMode_8E1 | UART_MR_CHMODE_NORMAL);
    TIMER_NAV_STEPPER.start(PERIOD_VERY_FAST_IT_US);
    TIMER_GPIO.start(PERIOD_FAST_IT_US);

    //Start all SW activities
    ArdOs::start();//this function never ends
}

void Robot2017::dieMotherFucker()
{
    LOG_INFO("Die mother fucker !!");

    //Ask the robot to stop moving and wait for it to be at rest
    nav.stopMoving();
    nav.wait();

    //last action as strategy thread is certainly called^^
#ifdef BUILD_STRATEGY
    lifecycle.stopThread();
#endif
    setRGBled(RED, FAST_BLINK);
    setLed(LED1, OFF);
    setLed(LED2, OFF);
    setLed(LED3, OFF);
    setLed(LED4, OFF);
}

bool Robot2017::isStartPlugged() const
{
    return hmi.tirette.read();
}

bool Robot2017::isColorSwitchOnPrefered() const
{
    return hmi.matchColor.read();
}

uint8_t Robot2017::getStrategyId() const
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

void Robot2017::setConfig(apb_Configuration const& newConf)
{
    m_params.setConfig(newConf);
    nav.updateConf(&m_params);
}

void Robot2017::sendSerialNumber()
{
    remoteControl.sendSerialNumber();
}

char const * const Robot2017::getSerialNumber() const
{
    return m_params.serialNumber();
}
