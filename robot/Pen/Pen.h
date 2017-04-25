/*
 * Pen.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_PEN_PEN_H_
#define ROBOT_PEN_PEN_H_

#include "../Common/2_AL/3_Strategies/0_Strategy2017.h"
#include "Robot2017.h"

namespace ard
{

    class Pen: public Robot2017
    {
    public:
        Pen();
        
        //retrieve the singleton instance (you should prefer the use of the g_ArdOs maccro)
        static Pen&
        getInstance()
        {
            if(instance2==NULL)
            {
                ASSERT(instance == NULL);
                instance2 = new Pen();
                instance = instance2;
            }

            return *instance2;
        };
        
    protected:
        static Pen* instance2;

#ifdef BUILD_STRATEGY
        HomolPen stratHomol;
        InstallPen stratInstall;
        Selftest stratSelftest;
        WipPen stratWIP;
#endif
    };

} /* namespace ard */



#endif /* ROBOT_PEN_PEN_H_ */
