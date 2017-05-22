/*
 * GpioTools.cpp
 *
 *  Created on: 21 oct. 2016
 *      Author: wix
 */

#include "Hmi.h"

using namespace ard;

/**
 * Configure the HMI buttons debounce time in ms.
 */
#define HMI_DEBOUNCE 50
#define RGB_PWM_PERIOD_US (10*1E3)
#define RGB_PWM_SO_YOU_KEEP_YOUR_EYES 110
/*--------------------------------------------------------------------- */
/*   OUTPUT                                                             */
/*--------------------------------------------------------------------- */

RgbLed::RgbLed(int pin_r, int pin_g, int pin_b):
        pwm_r(pin_r, RGB_PWM_PERIOD_US, true),
        pwm_g(pin_g, RGB_PWM_PERIOD_US, true),
        pwm_b(pin_b, RGB_PWM_PERIOD_US, true)
{
    m_color = eRgb::WHITE;
    m_blink = eLedState::OFF;
    output(m_blink);
    lastState = 0;
    m_on = false;
}

void RgbLed::set(eRgb color, eLedState blink)
{
    m_color = color;
    m_blink = blink;
}

void RgbLed::off()
{
    m_blink = eLedState::OFF;
}

void RgbLed::run()
{
    if ((lastState % 2 == 0 && m_blink == eLedState::FAST_BLINK) || (lastState % 10 == 0 && m_blink == eLedState::SLOW_BLINK))
        m_on = !m_on;
    else if (m_blink == eLedState::ON)
        m_on = true;
    else if (m_blink == eLedState::OFF)
        m_on = false;

    output(m_on);
    lastState++;
}

void RgbLed::output(bool _on)
{
    if (_on)
    {
        switch (m_color)
        {
        case eRgb::RED:
            pwm_r.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            pwm_g.setPWM(  0);
            pwm_b.setPWM(  0);
            break;
        case eRgb::GREEN:
            pwm_r.setPWM(  0);
            pwm_g.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            pwm_b.setPWM(  0);
            break;
        case eRgb::BLUE:
            pwm_r.setPWM(  0);
            pwm_g.setPWM(  0);
            pwm_b.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            break;
        case eRgb::YELLOW:
            pwm_r.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            pwm_g.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            pwm_b.setPWM(  0);
            break;
        case eRgb::CYAN:
            pwm_r.setPWM(  0);
            pwm_g.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            pwm_b.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            break;
        case eRgb::PURPLE:
            pwm_r.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            pwm_g.setPWM(  0);
            pwm_b.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            break;
        case eRgb::WHITE:
            pwm_r.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            pwm_g.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            pwm_b.setPWM(RGB_PWM_SO_YOU_KEEP_YOUR_EYES);
            break;
        case eRgb::MULTI:
            static uint16_t multi_R = 0;
            static uint16_t multi_G = 0;
            static uint16_t multi_B = 0;
            multi_R += 100;
            if( 1000 < multi_R )
            {
                multi_R = 0;
                multi_G += 100;
                if( 1000 < multi_G )
                {
                    multi_G = 0;
                    multi_B += 100;
                    if( 1000 < multi_B )
                        multi_B = 0;
                }
            }
            pwm_r.setPWM(multi_R);
            pwm_g.setPWM(multi_G);
            pwm_b.setPWM(multi_B);
            break;
        default:
            ;
        }
    }
    else
    {
        pwm_r.setPWM(0);
        pwm_g.setPWM(0);
        pwm_b.setPWM(0);
    }
}

uint8_t RgbLed::getState()
{
    return (pwm_r.read() << 2) + (pwm_g.read() << 1) + pwm_b.read();
}

//---------------------------------------------------------------------------

Led::Led(int pin, bool inverted)
        : m_blink(eLedState::OFF), m_pin(pin), m_inverted(inverted), lastState(0U), m_on(false)
{
}

void Led::on()
{
    m_blink = eLedState::ON;
}

void Led::slowBlink()
{
    m_blink = eLedState::SLOW_BLINK;
}

void Led::fastBlink()
{
    m_blink = eLedState::FAST_BLINK;
}

void Led::off()
{
    m_blink = eLedState::OFF;
}

void Led::run()
{
    if ((lastState % 3 == 0 && m_blink == eLedState::FAST_BLINK) || (lastState % 15 == 0 && m_blink == eLedState::SLOW_BLINK))
        m_on = !m_on;
    else if (m_blink == eLedState::ON)
        m_on = true;
    else if (m_blink == eLedState::OFF)
        m_on = false;

    if (m_inverted)
        digitalWrite(m_pin, 1 - m_on);
    else
        digitalWrite(m_pin, m_on);
    lastState++;
}

void Led::set(eLedState state)
{
    m_blink = state;
}

bool Led::getState()
{
    if (m_inverted)
        return 1 - digitalRead(m_pin);
    else
        return digitalRead(m_pin);
}

/*--------------------------------------------------------------------- */
/*   HMI                                                                */
/*--------------------------------------------------------------------- */
HmiThread::HmiThread(DueTimer& timer):
    Thread("Hmi", PRIO_HMI, STACK_HMI, PERIOD_HMI),
    ledRGB(LED_RGB_R, LED_RGB_G, LED_RGB_B),
    led1(LED1),
    led2(LED2),
    led3(LED3),
    led4(LED4),
    ledDue_Rx(LED_DUE_RX, INVERTED),
    ledDue_Tx(LED_DUE_TX, INVERTED),
    ledDue_L(LED_DUE_L),
    tirette(BUTTON_START, HMI_DEBOUNCE, HMI_DEBOUNCE, true),
    matchColor(BUTTON_COLOR, HMI_DEBOUNCE, HMI_DEBOUNCE),
    user1(BUTTON_USER1, HMI_DEBOUNCE, HMI_DEBOUNCE),
    user2(BUTTON_USER2, HMI_DEBOUNCE, HMI_DEBOUNCE),
    buzzer(timer, BUZZER, 150)
{
    state = apb_HmiState_init_default;
}

void HmiThread::run()
{
    ledRGB.run();
    led1.run();
    led2.run();
    led3.run();
    led4.run();
    ledDue_Rx.run();
    ledDue_Tx.run();
    ledDue_L.run();
}

bool HmiThread::isStartPlugged() const
{
	volatile bool test = tirette.read();
    return test;
}

bool HmiThread::isColorSwitchOnPrefered() const
{
    return matchColor.read();
}

uint8_t HmiThread::getStrategyId() const
{
    return (user1.read() << 1) + (user2.read());
}

void HmiThread::setRGBled(eRgb color, eLedState blink)
{
    ledRGB.set(color, blink);
}

void HmiThread::setLed(uint8_t led, eLedState blink)
{
    switch (led)
    {
    case LED1:
        led1.set(blink);
        break;

    case LED2:
        led2.set(blink);
        break;

    case LED3:
        led3.set(blink);
        break;

    case LED4:
        led4.set(blink);
        break;
    default:
        ASSERT_TEXT(false, "Unexpected value in setLed()");
        break;
    }
}

apb_HmiState const& HmiThread::serealize()
{
    state = apb_HmiState_init_default;
    state.led1 = led1.getState();
    state.led2 = led2.getState();
    state.led3 = led3.getState();
    state.led4 = led4.getState();
    state.ledRgb = ledRGB.getState();
    state.start = tirette.readRaw();
    state.colorSwitch = matchColor.readRaw();
    state.user1 = user1.readRaw();
    state.user2 = user2.readRaw();
    state.ledDue_Rx = ledDue_Rx.getState();
    state.ledDue_Tx = ledDue_Tx.getState();
    state.ledDue_L = ledDue_L.getState();
    return state;
}
