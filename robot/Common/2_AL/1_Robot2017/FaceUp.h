/*
 * FaceUp.h
 *
 *  Created on: 29 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_1_ROBOT2017_FACEUP_H_
#define ROBOT_COMMON_2_AL_1_ROBOT2017_FACEUP_H_

namespace ard
{
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
        FaceUp();

        //Turn the cylinder
        void faceUpCylinder();

        //Return the state of faceUpCylinderCmd()
        eFaceUpStatus getFaceUpStatus();
    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_1_ROBOT2017_FACEUP_H_ */
