/*
 * FaceUp.cpp
 *
 *  Created on: 29 avr. 2017
 *      Author: wix
 */

#include "../1_Robot/FaceUp.h"

using namespace ard;

FaceUp::FaceUp(ActuatorThread& parent):
    matchColor(eColor_UNKNOWN),
    acts(parent)
{
}

void FaceUp::setColor(eColor color)
{
    ASSERT_TEXT(matchColor == eColor_UNKNOWN, "You shouldn't configure color twice.");
    matchColor = color;
}

void FaceUp::faceUpCylinder()
{

}

eFaceUpStatus FaceUp::getFaceUpStatus()
{
    return FAILED;
}

