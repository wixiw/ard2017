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

    #include "generated/FSM_TrationBiBorder.h"
    namespace ard{
    class TrationBiBorder: public Strategy2017<FSM_TrationBiBorder, FSM_TrationBiBorder::FSM_TrationBiBorderStates>{
    public:
    	TrationBiBorder(Robot2017& robot, LSAList const& lsaList) :
        Strategy2017<FSM_TrationBiBorder, FSM_TrationBiBorder::FSM_TrationBiBorderStates>(robot, "HomolTration", lsaList){};
    };}

	#include "generated/FSM_TrationStartArea.h"
	namespace ard{
	class TrationStartArea: public Strategy2017<FSM_TrationStartArea, FSM_TrationStartArea::FSM_TrationStartAreaStates>{
	public:
		TrationStartArea(Robot2017& robot, LSAList const& lsaList) :
		Strategy2017<FSM_TrationStartArea, FSM_TrationStartArea::FSM_TrationStartAreaStates>(robot, "HomolTration", lsaList){};
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

        TrationBiBorder stratBiBorder;
        TrationStartArea stratStartArea;
        InstallTration stratInstall;
        Selftest stratSelftest;

    protected:
        String exeBuildDate;
    };

} /* namespace ard */



#endif /* ROBOT_TRATION_TRATION_H_ */
