/*
 * Tration.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_TRATION_TRATION_H_
#define ROBOT_TRATION_TRATION_H_

#include "../Common/2_AL/3_Strategies/0_Strategy2017.h"
#include "Robot2017.h"

namespace ard
{

    class Tration: public Robot2017
    {
    public:
        Tration();
        
        //retrieve the singleton instance (you should prefer the use of the g_ArdOs maccro)
        static Tration&
        getInstance()
        {
            if(instance2==NULL)
            {
                ASSERT(instance == NULL);
                instance2 = new Tration();
                instance = instance2;
            }

            return *instance2;
        };
        
    protected:
        static Tration* instance2;

#ifdef BUILD_STRATEGY
        InstallTration stratInstall;
        Selftest stratSelftest;
        HomolTration stratHomol;
        WipTration stratWIP;
        FunnyAction stratFunnyAction;
#endif
    };

} /* namespace ard */



#endif /* ROBOT_TRATION_TRATION_H_ */
