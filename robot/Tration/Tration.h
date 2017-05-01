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
        
        //Overrides Robot20017
        void init(Robot2017Listener* client) override;

        //Implements Robot2017
        String const& getExeVersion() override;

        HomolTration stratHomol;
        InstallTration stratInstall;
        Selftest stratSelftest;
        WipTration stratWIP;
        FunnyAction stratFunnyAction;

    protected:
        String exeBuildDate;
    };

} /* namespace ard */



#endif /* ROBOT_TRATION_TRATION_H_ */
