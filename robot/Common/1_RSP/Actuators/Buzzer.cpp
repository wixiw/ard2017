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
#include "BSP.h"

#define BPM 120    //  you can change this value changing all the others
#define H 2*Q //half 2/4
#define Q 60000/BPM //quarter 1/4
#define E Q/2   //eighth 1/8
#define S Q/4 // sixteenth 1/16
#define W 4*Q // whole 4/4


using namespace ard;

//The default freq is adjusted to be the louder freq in Hz
#define DEFAULT_FREQ 3000U
//The default tone duration in ms
#define DEFAULT_DURATION 100U
//The default duration between two bips in ms
#define DEFAULT_INTERTONE 100U

//In order to map an interrupt to a C++ class
static Buzzer* buzInst = NULL;

void Buzzer_Handler(void)
{
    //DEBUG_SET_HIGH(); //uncomment to check period and delay with oscilloscope

    ASSERT(buzInst);
    buzInst->interrupt();

    //DEBUG_SET_LOW(); //uncomment to check period and delay with oscilloscope : default empty duration (2 io write + 1 function call) is 750ns
}

Buzzer::Buzzer(DueTimer& timer, uint8_t pin, uint16_t queueSize)
        : pin(pin),
          timer(timer),
          queue(queueSize, sizeof(Tone)),
          currentToneCpt(0),
          m_soundPlayed(true)
{
    ASSERT(buzInst == NULL);
    buzInst = this;
    timer.attachInterrupt(Buzzer_Handler);
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
    m_soundPlayed = false;
    queue.push(&tone, 0);
    timer.start(10000);//start timer 10ms after having received data.
}

void Buzzer::silence(uint16_t lengthMs)
{
    playTone(ToneSilence(lengthMs));
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

bool Buzzer::soundPlayed()
{
    return m_soundPlayed;
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
                timer.stop();
                timer.setFrequency(2*tone.frequency);
                timer.start();
            }
            else
            {
                timer.stop();
                timer.setPeriod(tone.lengthMs*1000);
                timer.start();
            }
        }
        else
        {
            //queue vide
            timer.stop();
            timer.setPeriod(10000);
            timer.start();
            digitalWrite(pin, 0);
            m_soundPlayed = true;
            empty.set();
        }
    }
}

void Buzzer::naderBell()
{
    static bool state = true;

    if(m_soundPlayed && state)
    {
        playTone(3000, 1000);
        state = !state;
    }
    if(m_soundPlayed && !state)
    {
        silence(500);
    }
}

void ard::Buzzer::playSharp(uint16_t frequency, uint16_t lengthMs)
{
    playTone(frequency, lengthMs - 50);
    silence(50); //to separate notes
}

void Buzzer::imperialMarch()
{
    playSharp(LA3, Q);
    playSharp(LA3, Q);
    playSharp(LA3, Q);
    playSharp(F3,  E+S);
    playSharp(C4,  S);

    playSharp(LA3, Q);
    playSharp(F3,  E+S);
    playSharp(C4,  S);
    playSharp(LA3, H);

    playSharp(E4,  Q);
    playSharp(E4,  Q);
    playSharp(E4,  Q);
    playSharp(F4,  E+S);
    playSharp(C4,  S);

    playSharp(Ab3, Q);
    playSharp(F3,  E+S);
    playSharp(C4,  S);
    playSharp(LA3, H);

    playSharp(LA4, Q);
    playSharp(LA3, E+S);
    playSharp(LA3, S);
    playSharp(LA4, Q);
    playSharp(Ab4, E+S);
    playSharp(G4,  S);

    playSharp(Gb4, S);
    playSharp(E4,  S);
    playSharp(F4,  E);
    silence(E);
    playSharp(Bb3, E);
    playSharp(Eb4, Q);
    playSharp(D4,  E+S);
    playSharp(Db4, S);

    playSharp(C4,  S);
    playSharp(B3,  S);
    playSharp(C4,  E);
    silence(E);
    playSharp(F3,  E);
    playSharp(Ab3, Q);
    playSharp(F3,  E+S);
    playSharp(LA3, S);

    playSharp(C4,  Q);
    playSharp(LA3, E+S);
    playSharp(C4,  S);
    playSharp(E4,  H);
//truc
    playSharp(LA4, Q);
    playSharp(LA3, E+S);
    playSharp(LA3, S);
    playSharp(LA4, Q);
    playSharp(Ab4, E+S);
    playSharp(G4,  S);

    playSharp(Gb4, S);
    playSharp(E4,  S);
    playSharp(F4,  E);
    silence(E);
    playSharp(Bb3, E);
    playSharp(Eb4, Q);
    playSharp(D4,  E+S);
    playSharp(Db4, S);

    playSharp(C4,  S);
    playSharp(B3,  S);
    playSharp(C4,  E);
    silence(E);
    playSharp(F3,  E);
    playSharp(Ab3, Q);
    playSharp(F3,  E+S);
    playSharp(LA3, S);

    playSharp(LA3, Q);
    playSharp(F3,  E+S);
    playSharp(C4,  S);
    playSharp(LA3, H);
}
