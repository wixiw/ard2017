/*
 * Pen.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_PEN_PEN_H_
#define ROBOT_PEN_PEN_H_

#include "AL.h"

/**
 * The list of awsome strategies to beat RCVA
 * If you read this before the contest you are cheating, it's bad !
 * However we'll break the strategy the last night as always to create a brand new one,
 * so don't expect anything about the current information.
 *
 * Yes, I'm saying bullshit, but's it's 3h since I'm trying to make a template to compile.
 * Bad life.
 */
#define private public //#porky : workaround to gain introspection, because Yakindu guys thinks they'll anoy me in making data private...

#include "generated/FSM_HomolPen.h"
    namespace ard{
    class HomolPen: public Strategy2017<FSM_HomolPen, FSM_HomolPen::FSM_HomolPenStates>{
    public:
        HomolPen(Robot2017& robot, LSAList const& lsaList) :
        Strategy2017<FSM_HomolPen, FSM_HomolPen::FSM_HomolPenStates>(robot, "HomolPen", lsaList){};

        void rouleau(sc_boolean descendu) override
        {
        	if(descendu)
        		robot.actuators.servo8.goTo(500); //TODO
        	else
        		robot.actuators.servo8.goTo(500); //TODO
        }
    };}

//#include "generated/FSM_WipPen.h"
//DECLARE_FSM_STRATEGY(WipPen);

#undef private

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

        //Implements Robot2017
        LSA& getLSA(uint8_t id) const override;

        //LSA
        LSAList lsaList;

        //Strategy layer
        HomolPen stratHomol;
        InstallPen stratInstall;
        Selftest stratSelftest;
        //WipPen stratWIP;
        FunnyAction stratFunnyAction;

    protected:
        String exeBuildDate;
    };

} /* namespace ard */



#endif /* ROBOT_PEN_PEN_H_ */
