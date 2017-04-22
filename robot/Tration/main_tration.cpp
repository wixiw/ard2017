/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "Robot2017.h"
#include "3_Strategies/Strategies.h"

using namespace ard;

/**
 * The little 2016 PMI
 */

int main( void )
{   
    Robot2017& robotTration = Robot2017::getInstance();

#ifdef BUILD_STRATEGY
    FunnyAction stratFunnyAction(&robotTration);
    Homol stratHomol(&robotTration);
    InstallPen stratInstallation(&robotTration);
    Invade stratInvade(&robotTration);
    Match stratMatch(&robotTration);
    Quentin stratQuentin(&robotTration);
    Selftest stratSelftest(&robotTration);
    Tanguy stratTanguy(&robotTration);
    Willy stratWilly(&robotTration);


    //Build and start robot
    robotTration.bootOs();

    return 0;
}

extern String getExeVersion()
{
    return String("Version Tration : ") + __DATE__ + " " + __TIME__;
}

