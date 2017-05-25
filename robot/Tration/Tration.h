/*
 * Tration.h
 *
 *  Created on: 22 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_TRATION_TRATION_H_
#define ROBOT_TRATION_TRATION_H_

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

    #include "generated/FSM_HomolTration.h"
    namespace ard{
    class HomolTration: public Strategy2017<FSM_HomolTration, FSM_HomolTration::FSM_HomolTrationStates>{
    public:
        HomolTration(Robot2017& robot, LSAList const& lsaList) :
        Strategy2017<FSM_HomolTration, FSM_HomolTration::FSM_HomolTrationStates>(robot, "HomolTration", lsaList){};

        void ackCylinderTaken() override
        {
        	robot.actuators.arms.ackCylinderTaken();
        }

        //Specific FSM LSA //TODO a mettre en standard
        void recalRearSpecial(sc_integer border, sc_integer distance) override
        {
            robot.nav.recalRear((eTableBorder)(border), distance);
        }
    };}

//    #include "generated/FSM_WipTration.h"
//    DECLARE_FSM_STRATEGY(WipTration);

#undef private

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

        //Implements Robot2017
        LSA& getLSA(uint8_t id) const override;

        //LSA
        LSAList lsaList;

        HomolTration stratHomol;
        InstallTration stratInstall;
        Selftest stratSelftest;
        //WipTration stratWIP;
        FunnyAction stratFunnyAction;

    protected:
        String exeBuildDate;
    };

} /* namespace ard */



#endif /* ROBOT_TRATION_TRATION_H_ */
