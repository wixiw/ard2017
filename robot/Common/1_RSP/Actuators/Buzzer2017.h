/*
 * Buzzer2017.h
 *
 *  Created on: 26 mars 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_BUZZER2017_H_
#define ROBOT_COMMON_2_AL_BUZZER2017_H_

#include "Buzzer.h"

namespace ard
{

    class Buzzer2017 : public Buzzer
    {
    public:
        Buzzer2017(DueTimer& timer);
        
        void playSharp(uint16_t frequency, uint16_t lengthMs);

        void naderBell(); //rear move warning
        void imperialMarch();

    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_BUZZER2017_H_ */
