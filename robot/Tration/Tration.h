/*
 * Tration.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_TRATION_TRATION_H_
#define ROBOT_TRATION_TRATION_H_

#include "Robot2017.h"
#include "3_Strategies/Strategies.h"

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
        FunnyAction stratFunnyAction;
        InstallPen stratInstallation;
        Invade stratInvade;
        WipTration stratWIP;
#endif
    };

} /* namespace ard */



#endif /* ROBOT_TRATION_TRATION_H_ */
