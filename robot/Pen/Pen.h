/*
 * Pen.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_PEN_PEN_H_
#define ROBOT_PEN_PEN_H_

#include "Robot2017.h"
#include "3_Strategies/Strategies.h"

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
        Homol stratHomol;
        InstallPen stratInstallation;
        Match stratMatch;
        Selftest stratSelftest;
        WipPen stratWIP;
#endif
    };

} /* namespace ard */



#endif /* ROBOT_PEN_PEN_H_ */
