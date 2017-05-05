/*
 * ArdFramework.cpp
 *
 *  Created on: 15 janv. 2017
 *      Author: wix
 */

#include "ArdFramework.h"

using namespace ard;

String ard::color2String(eColor color)
{
    switch (color) {
        case eColor_PREF:
            return String("PREF (Yellow)");
            break;
        case eColor_SYM:
            return String("SYM (Blue)");
            break;
        case eColor_UNKNOWN:
            return String("UNKNOWN");
            break;
        default:
            ASSERT(false);
            return String("DEFAULT");
            break;
    }
}

ArdObject::ArdObject(String const& name):
        initialized(false),
        name(name)
{
}

void ArdObject::init()
{
    ASSERT(!initialized); //init already done
    initialized = true;
}

//It has to be here for some compiler reasons
ILogger::~ILogger(){}
ArdObject::~ArdObject(){};


