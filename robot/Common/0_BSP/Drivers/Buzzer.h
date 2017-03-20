/*
 * Buzzer.h
 *
 *  Created on: 16 mars 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_0_BSP_DRIVERS_BUZZER_H_
#define ROBOT_COMMON_0_BSP_DRIVERS_BUZZER_H_

#include "DueTimer.h"
#include "ArdOs.h"

namespace ard
{
    struct Tone
    {
        uint16_t frequency; //Hz
        uint8_t lengthMs; //Ms
    };

    class Buzzer
    {       
    public:
        //Only one buzzer can be instanciated
        //@param timer : the timer reference used to generate the signal
        //@param pin : the pin on which the tone is generated
        //@param queueSize : maximal nb tunes to queue
        Buzzer(DueTimer& timer, uint8_t pin, uint16_t queueSize);

        //Start or stop the buzzer (continous sound), it also empty the sound queue
        void buzz(bool on);

        //Send a train of bips to the sound queue
        void bip(uint8_t nb);

        //Send a tone to the buzzer
        void playTone(uint16_t frequency, uint8_t lengthMs);
        void playTone(Tone const& tone);

        //Play a series a notes
        void playMelody(Tone const* melody, uint16_t nbTones);

        //wait until all sounds has been processed
        void wait();

        void interrupt();
        
    protected:
        void emptyQueue();
        
    private:
        uint8_t pin;
        DueTimer& timer;
        Queue queue;
        uint32_t currentToneCpt;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_0_BSP_DRIVERS_BUZZER_H_ */
