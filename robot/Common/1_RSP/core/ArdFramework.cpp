/*
 * ArdFramework.cpp
 *
 *  Created on: 15 janv. 2017
 *      Author: wix
 */

#include "ArdFramework.h"

using namespace ard;



ArdObject::ArdObject(String const& name):
        initialized(false),
        name(name)
{
    //TODO assert
}

void ArdObject::init()
{
    ASSERT(!initialized); //init already done
    initialized = true;
}

//It has to be here for some compiler reasons
ILogger::~ILogger(){}
ArdObject::~ArdObject(){};


