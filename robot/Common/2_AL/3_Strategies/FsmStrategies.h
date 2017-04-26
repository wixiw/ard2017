/*
 * FsmStrategies.h
 *
 *  Created on: 26 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_3_STRATEGIES_FSMSTRATEGIES_H_
#define ROBOT_COMMON_2_AL_3_STRATEGIES_FSMSTRATEGIES_H_

#include "0_Strategy2017.h"

/**
 * Use this macro to automatically declare an FSM strategy as long as :
 * - you named the machine file FSM_[stratName].sct
 * - the machine name inisde the file is FSM_[stratName]
 * - you wish to name the created class [stratName]
 *
 * This macro is too nice. Batman out! ....  Oh shit, 'forgot to drop the mic...
 */
    #define DECLARE_FSM_STRATEGY(stratName)\
    namespace ard {\
    class stratName: public Strategy2017<FSM_##stratName, FSM_##stratName::FSM_##stratName##States>{\
    public:\
        stratName(Robot2017& robot) : Strategy2017<FSM_##stratName, FSM_##stratName::FSM_##stratName##States>(robot, #stratName){};\
        String state2Str(FSM_##stratName::FSM_##stratName##States state) const;\
    };}


/**
 * The list of awsome strategies to beat RCVA
 * If you read this before the contest you are cheating, it's bad !
 * However we'll break the strategy the last night as always to create a brand new one,
 * so don't expect anything about the current information.
 *
 * Yes, I'm saying bullshit, but's it's 3h since I'm trying to make a template to compile.
 * Bad life.
 */
#define private public //workaround to gain introspection, because Yakindu guys thinks they'll anoy me in making data private...

    #include "generated/FSM_SelfTest.h"
    DECLARE_FSM_STRATEGY(Selftest);

    #include "generated/FSM_HomolPen.h"
    DECLARE_FSM_STRATEGY(HomolPen);

    #include "generated/FSM_HomolTration.h"
    DECLARE_FSM_STRATEGY(HomolTration);

    #include "generated/FSM_InstallPen.h"
    DECLARE_FSM_STRATEGY(InstallPen);

    #include "generated/FSM_InstallTration.h"
    DECLARE_FSM_STRATEGY(InstallTration);

    #include "generated/FSM_WipPen.h"
    DECLARE_FSM_STRATEGY(WipPen);

    #include "generated/FSM_WipTration.h"
    DECLARE_FSM_STRATEGY(WipTration);


#undef private
#endif /* ROBOT_COMMON_2_AL_3_STRATEGIES_FSMSTRATEGIES_H_ */
