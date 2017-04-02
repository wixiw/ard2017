/*
 * Buzzer2017.cpp
 *
 *  Created on: 26 mars 2017
 *      Author: wix
 */

#include "Buzzer2017.h"
#include "BSP.hpp"

using namespace ard;

#define BPM 120    //  you can change this value changing all the others
#define H 2*Q //half 2/4
#define Q 60000/BPM //quarter 1/4
#define E Q/2   //eighth 1/8
#define S Q/4 // sixteenth 1/16
#define W 4*Q // whole 4/4

Buzzer2017::Buzzer2017(DueTimer& timer):
	Buzzer(timer, BUZZER, 150) //queue size to be adjusted with CommonMsg.proto Melody size
{
}

void Buzzer2017::naderBell()
{
    playTone(3000, 1000);
    silence(500);
    playTone(3000, 1000);
    silence(500);
    playTone(3000, 1000);
    silence(500);
}

void ard::Buzzer2017::playSharp(uint16_t frequency, uint16_t lengthMs)
{
    playTone(frequency, lengthMs - 50);
    silence(50); //to separate notes
}

void Buzzer2017::imperialMarch()
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
