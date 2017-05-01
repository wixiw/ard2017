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

        //Overrides Robot20017
        void init(Robot2017Listener* client) override;

        //Implements Robot2017
        String const& getExeVersion() override;

        //Strategy layer
        HomolPen stratHomol;
        InstallPen stratInstall;
        Selftest stratSelftest;
        WipPen stratWIP;

    protected:
        String exeBuildDate;
    };

} /* namespace ard */



#endif /* ROBOT_PEN_PEN_H_ */
