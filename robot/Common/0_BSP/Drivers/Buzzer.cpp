/*
 * Buzzer.cpp
 *
 *  Created on: 16 mars 2017
 *      Author: wix
 */

/* Tone generation for DUE
 see http://arduino.cc/forum/index.php/topic,136500.msg1029238.html#msg1029238 and
 http://asf.atmel.com/docs/latest/sam3a/html/group__sam__drivers__pmc__group.html and
 http://asf.atmel.com/docs/latest/sam.drivers.usart.usart_synchronous_example.sam3u_ek/html/sam_pio_quickstart.html
 and http://ko7m.blogspot.com.au/2015/01/arduino-due-timers-part-1.html
 */

#include "Buzzer.h"
#include "ArdOs.h"

using namespace ard;

//The default freq is adjusted to be the louder freq in Hz
#define DEFAULT_FREQ 3000U
//The default tone duration in ms
#define DEFAULT_DURATION 10000U
//The default duration between two bips in ms
#define DEFAULT_INTERTONE 200000U

//In order to map an interrupt to a C++ class
static Buzzer* buzInst = NULL;

void Buzzer_Handler(void)
{
    ASSERT(buzInst);
    buzInst->interrupt();
}

Buzzer::Buzzer(DueTimer& timer, uint8_t pin, uint16_t queueSize)
        : pin(pin), timer(timer),queue(queueSize, sizeof(Tone)), currentToneCpt(0)
{
    ASSERT(buzInst == NULL);
    buzInst = this;
    timer.attachInterrupt(Buzzer_Handler);
}

void Buzzer::buzz(bool on)
{
    emptyQueue();

    if(on)
        playTone(DEFAULT_FREQ, DEFAULT_DURATION);
}

void Buzzer::bip(uint8_t nb)
{
    for(int i = 0 ; i < nb ; i++)
    {
        playTone(DEFAULT_FREQ, DEFAULT_DURATION);
        playTone(ToneSilence(DEFAULT_INTERTONE));
    }
}

void Buzzer::playTone(uint16_t frequency, uint16_t lengthMs)
{
    Tone newTone;
    newTone.frequency = frequency;
    newTone.lengthMs = lengthMs;
    playTone(newTone);
}

void Buzzer::playTone(Tone const& tone)
{
    queue.push(&tone, 0);
    timer.start(1);
}

void Buzzer::playMelody(Melody melody, uint16_t nbTones)
{
    for(int i = 0 ; i < nbTones ; ++i)
    {
        playTone(melody[i]);
    }
}

void Buzzer::wait()
{
    empty.wait();
}

void Buzzer::emptyQueue()
{
    //TODO empty queue
    ASSERT(false);
}

void Buzzer::interrupt()
{
    if (currentToneCpt != 0)
    {
        digitalWrite(pin, !digitalRead(pin));     // invert the pin state (i.e toggle)
        if (currentToneCpt > 0)
            currentToneCpt--;
    }
    else
    {
        //depiler queue
        Tone tone;
        if( queue.pop(&tone) )
        {
            if(tone.frequency)
            {
                currentToneCpt = (2 * tone.frequency * tone.lengthMs) / 1000; // calculate no of waveform edges (rises/falls) for the tone burst
                timer.setFrequency(tone.frequency);
            }
            else
            {
                timer.setPeriod(tone.lengthMs*1000);
            }
        }
        else
        {
            //queue vide
            timer.stop();
            digitalWrite(pin, 0);
            empty.set();
        }
    }
}
