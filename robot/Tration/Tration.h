/*
 * Tration.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_TRATION_TRATION_H_
#define ROBOT_TRATION_TRATION_H_

#include "AL.h"

namespace ard
{

    class Tration: public Robot2017
    {
    public:
        Tration();
        
        HomolTration stratHomol;
        InstallTration stratInstall;
        Selftest stratSelftest;
        WipTration stratWIP;
        FunnyAction stratFunnyAction;
    };

} /* namespace ard */



#endif /* ROBOT_TRATION_TRATION_H_ */
