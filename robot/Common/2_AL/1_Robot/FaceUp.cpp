/*
 * FaceUp.cpp
 *
 *  Created on: 29 avr. 2017
 *      Author: wix
 */

#include "FaceUp.h"

using namespace ard;

FaceUp::FaceUp(ActuatorThread& parent):
    matchColor(eColor_UNKNOWN),
    acts(parent)
{
}

void FaceUp::setColor(eColor color)
{
    ASSERT_TEXT(color != eColor_UNKNOWN, "You shouldn't configure color with unknown.");
    matchColor = color;
}

void FaceUp::faceUpCylinder()
{

}

eFaceUpStatus FaceUp::getFaceUpStatus()
{
    return FAILED;
}

