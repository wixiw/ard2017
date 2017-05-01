/*
 * FaceUp.h
 *
 *  Created on: 29 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_1_ROBOT_FACEUP_H_
#define ROBOT_COMMON_2_AL_1_ROBOT_FACEUP_H_

#include "RSP.h"

namespace ard
{
    class ActuatorThread;

    //CAUTION : keep in sync with state machines API
    typedef enum
    {
        NO_CMD,
        FAILED,
        OK
    } eFaceUpStatus;

    class FaceUp
    {
    public:
        FaceUp(ActuatorThread& parent);

        //Selects the match color
        void setColor(eColor color);

        //Turn the cylinder
        void faceUpCylinder();

        //Return the state of faceUpCylinderCmd()
        eFaceUpStatus getFaceUpStatus();

    private:
        eColor        matchColor;

        //Actuators list
        ActuatorThread& acts;
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_1_ROBOT_FACEUP_H_ */
