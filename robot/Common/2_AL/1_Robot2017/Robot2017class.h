/*
 * Robot2017.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_Robot2017_Robot2017_H_
#define ROBOT_Robot2017_Robot2017_H_

#include "YakardTimer.h"
#include "StrategyModel2017.h"
#include "Lifecycle.h"
#include "ActuatorThread.h"
#include "RemoteControl.h"

namespace ard
{

    class Robot2017: public Robot
    {
    public:
        Robot2017();
        
        //Implements Robot
        virtual void dieMotherFucker() override;

        //Implements Robot
        virtual void sendSerialNumber() override;

        //Implements Robot
        virtual void setMatchColor(eColor color) override;

#ifdef BUILD_REMOTE_CONTROL
        RemoteControl remoteControl;
#endif
        StrategyModel2017 stratInfo;
        ActuatorThread actuators;
        Lifecycle lifecycle;
    };

} /* namespace ard */



#endif /* ROBOT_Robot2017_Robot2017_H_ */
