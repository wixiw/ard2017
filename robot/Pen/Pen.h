/*
 * Pen.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_PEN_PEN_H_
#define ROBOT_PEN_PEN_H_

#include "AL.h"

namespace ard
{
    class Pen: public Robot2017
    {
    public:
        Pen();
        
        //Strategy layer
        HomolPen stratHomol;
        InstallPen stratInstall;
        Selftest stratSelftest;
        WipPen stratWIP;

    };

} /* namespace ard */



#endif /* ROBOT_PEN_PEN_H_ */
