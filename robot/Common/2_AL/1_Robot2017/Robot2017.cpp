/*
 * Robot2017.cpp
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#include "Robot2017class.h"

using namespace ard;

Robot2017::Robot2017():
    Robot(),
#ifdef BUILD_REMOTE_CONTROL
    remoteControl(this, bsp.serial0),
#endif
    stratInfo(),
    actuators(),
    lifecycle(this)
{
#ifdef BUILD_REMOTE_CONTROL
    log.addLogger(remoteControl);
#endif

}

void Robot2017::dieMotherFucker()
{
    LOG_INFO("Die mother fucker !!");

    //Ask the robot to stop moving and wait for it to be at rest
    nav.stopMoving();
    actuators.disableAll();

    //Play with LED for fun
    setRGBled(RED, FAST_BLINK);
    setLed(LED1, OFF);
    setLed(LED2, OFF);
    setLed(LED3, OFF);
    setLed(LED4, OFF);
}

void Robot2017::sendSerialNumber()
{
    remoteControl.sendSerialNumber();
}

void Robot2017::setMatchColor(eColor color)
{
    if ( color == eColor_PREF )
    {
        setRGBled(YELLOW, ON);
        nav.setColor (eColor_PREF);
        stratInfo.setColor(eColor_PREF);
        LOG_INFO("User has selected PREF (Yellow) color.");
    }
    else if ( color == eColor_SYM )
    {
        setRGBled(BLUE, ON);
        nav.setColor (eColor_SYM);
        stratInfo.setColor(eColor_SYM);
        LOG_INFO("User has selected SYM (Blue) color.");
    }
    else
    {
        ASSERT_TEXT(false, "StrategyThread::configureMatch : color should not be unknown.");
    }
}
