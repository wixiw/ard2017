
#include "FSM_WipPen.h"
#include <string.h>

/*! \file Implementation of the state machine 'FSM_WipPen'
*/

FSM_WipPen::FSM_WipPen()
{
	
	iface_OCB = null;
	
	stateConfVectorPosition = 0;
	
	timer = null;
}

FSM_WipPen::~FSM_WipPen()
{
}


void FSM_WipPen::init()
{
	for (int i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = FSM_WipPen_last_state;
	
	stateConfVectorPosition = 0;

	clearInEvents();
	clearOutEvents();
	
	/* Default init sequence for statechart FSM_WipPen */
	ifaceInternalSCI.FWD = 1;
	ifaceInternalSCI.BWD = -1;
	ifaceInternalSCI.TOP_Y = 0;
	ifaceInternalSCI.OWN_SIDE_X = 1;
	ifaceInternalSCI.OPP_SIDE_X = 2;
	ifaceInternalSCI.START_AREA_Y = 3;
	ifaceInternalSCI.BOT_Y = 4;
	ifaceInternalSCI.START_AREA_X = 5;
	ifaceInternalSCI.LED_OFF = 0;
	ifaceInternalSCI.LED_ON = 1;
	ifaceInternalSCI.SLOW_BLINK = 2;
	ifaceInternalSCI.FAST_BLINK = 3;
	ifaceInternalSCI.ledRED = 0;
	ifaceInternalSCI.ledGREEN = 1;
	ifaceInternalSCI.ledBLUE = 2;
	ifaceInternalSCI.ledYELLOW = 3;
	ifaceInternalSCI.ledCYAN = 4;
	ifaceInternalSCI.ledPURPLE = 5;
	ifaceInternalSCI.ledWHITE = 6;
	ifaceInternalSCI.cylUNKNOWN = 0;
	ifaceInternalSCI.cylYELLOW = 1;
	ifaceInternalSCI.cylBLUE = 2;
	ifaceInternalSCI.cylWHITE = 3;
	ifaceInternalSCI.NO_CMD = 0;
	ifaceInternalSCI.FAILED = 1;
	ifaceInternalSCI.OK = 2;
	ifaceInternalSCI.NO_LSA = 0;
	ifaceInternalSCI.SUCCESS = 1;
	ifaceInternalSCI.FAILURE = 2;
	ifaceInternalSCI.IN_PROGRESS = 3;
	ifaceInternalSCI.LSA_A = 0;
	ifaceInternalSCI.LSA_G = 1;
	ifaceInternalSCI.LSA_OppG = 2;
	ifaceInternalSCI.LSA_1 = 3;
	ifaceInternalSCI.LSA_2 = 4;
	ifaceInternalSCI.LSA_3 = 5;
	ifaceInternalSCI.LSA_4 = 6;
	ifaceInternalSCI.LSA_5 = 7;
	ifaceInternalSCI.LSA_B = 8;
	ifaceInternalSCI.LSA_C = 9;
	ifaceInternalSCI.LSA_D = 10;
	ifaceInternalSCI.LSA_E = 11;
	ifaceInternalSCI.LSA_F = 12;
	ifaceInternalSCI.LSA_OppD = 13;
	ifaceInternalSCI.LSA_OppE = 14;
	ifaceInternalSCI.LSA_OppF = 15;
	ifaceInternalSCI.LSA_6 = 16;
}

void FSM_WipPen::enter()
{
	/* Default enter sequence for statechart FSM_WipPen */
	enseq_main_region_default();
}

void FSM_WipPen::exit()
{
	/* Default exit sequence for statechart FSM_WipPen */
	exseq_main_region();
}

sc_boolean FSM_WipPen::isActive()
{
	return stateConfVector[0] != FSM_WipPen_last_state;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean FSM_WipPen::isFinal()
{
   return false;}

void FSM_WipPen::runCycle()
{
	
	clearOutEvents();
	
	for (stateConfVectorPosition = 0;
		stateConfVectorPosition < maxOrthogonalStates;
		stateConfVectorPosition++)
		{
			
		switch (stateConfVector[stateConfVectorPosition])
		{
		case main_region_StateA :
		{
			react_main_region_StateA();
			break;
		}
		case main_region_StateB :
		{
			react_main_region_StateB();
			break;
		}
		case main_region_LSA :
		{
			react_main_region_LSA();
			break;
		}
		case main_region_Recal_Front :
		{
			react_main_region_Recal_Front();
			break;
		}
		case main_region_Recal_Rear :
		{
			react_main_region_Recal_Rear();
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents();
}

void FSM_WipPen::clearInEvents()
{
	timeEvents[0] = false; 
}

void FSM_WipPen::clearOutEvents()
{
}


void FSM_WipPen::setTimer(TimerInterface* timer)
{
	this->timer = timer;
}

TimerInterface* FSM_WipPen::getTimer()
{
	return timer;
}

void FSM_WipPen::raiseTimeEvent(sc_eventid evid)
{
	if ((evid >= (sc_eventid)timeEvents) && (evid < (sc_eventid)(&timeEvents[timeEventsCount])))
	{
		*(sc_boolean*)evid = true;
	}				
}

sc_boolean FSM_WipPen::isStateActive(FSM_WipPenStates state)
{
	switch (state)
	{
		case main_region_StateA : 
			return (sc_boolean) (stateConfVector[0] == main_region_StateA
			);
		case main_region_StateB : 
			return (sc_boolean) (stateConfVector[0] == main_region_StateB
			);
		case main_region_LSA : 
			return (sc_boolean) (stateConfVector[0] == main_region_LSA
			);
		case main_region_Recal_Front : 
			return (sc_boolean) (stateConfVector[0] == main_region_Recal_Front
			);
		case main_region_Recal_Rear : 
			return (sc_boolean) (stateConfVector[0] == main_region_Recal_Rear
			);
		default: return false;
	}
}

FSM_WipPen::DefaultSCI* FSM_WipPen::getDefaultSCI()
{
	return &iface;
}


void FSM_WipPen::setDefaultSCI_OCB(DefaultSCI_OCB* operationCallback)
{
	iface_OCB = operationCallback;
}

sc_integer FSM_WipPen::InternalSCI::get_fWD()
{
	return FWD;
}

sc_integer FSM_WipPen::InternalSCI::get_bWD()
{
	return BWD;
}

sc_integer FSM_WipPen::InternalSCI::get_tOP_Y()
{
	return TOP_Y;
}

sc_integer FSM_WipPen::InternalSCI::get_oWN_SIDE_X()
{
	return OWN_SIDE_X;
}

sc_integer FSM_WipPen::InternalSCI::get_oPP_SIDE_X()
{
	return OPP_SIDE_X;
}

sc_integer FSM_WipPen::InternalSCI::get_sTART_AREA_Y()
{
	return START_AREA_Y;
}

sc_integer FSM_WipPen::InternalSCI::get_bOT_Y()
{
	return BOT_Y;
}

sc_integer FSM_WipPen::InternalSCI::get_sTART_AREA_X()
{
	return START_AREA_X;
}

sc_integer FSM_WipPen::InternalSCI::get_lED_OFF()
{
	return LED_OFF;
}

sc_integer FSM_WipPen::InternalSCI::get_lED_ON()
{
	return LED_ON;
}

sc_integer FSM_WipPen::InternalSCI::get_sLOW_BLINK()
{
	return SLOW_BLINK;
}

sc_integer FSM_WipPen::InternalSCI::get_fAST_BLINK()
{
	return FAST_BLINK;
}

sc_integer FSM_WipPen::InternalSCI::get_ledRED()
{
	return ledRED;
}

sc_integer FSM_WipPen::InternalSCI::get_ledGREEN()
{
	return ledGREEN;
}

sc_integer FSM_WipPen::InternalSCI::get_ledBLUE()
{
	return ledBLUE;
}

sc_integer FSM_WipPen::InternalSCI::get_ledYELLOW()
{
	return ledYELLOW;
}

sc_integer FSM_WipPen::InternalSCI::get_ledCYAN()
{
	return ledCYAN;
}

sc_integer FSM_WipPen::InternalSCI::get_ledPURPLE()
{
	return ledPURPLE;
}

sc_integer FSM_WipPen::InternalSCI::get_ledWHITE()
{
	return ledWHITE;
}

sc_integer FSM_WipPen::InternalSCI::get_cylUNKNOWN()
{
	return cylUNKNOWN;
}

sc_integer FSM_WipPen::InternalSCI::get_cylYELLOW()
{
	return cylYELLOW;
}

sc_integer FSM_WipPen::InternalSCI::get_cylBLUE()
{
	return cylBLUE;
}

sc_integer FSM_WipPen::InternalSCI::get_cylWHITE()
{
	return cylWHITE;
}

sc_integer FSM_WipPen::InternalSCI::get_nO_CMD()
{
	return NO_CMD;
}

sc_integer FSM_WipPen::InternalSCI::get_fAILED()
{
	return FAILED;
}

sc_integer FSM_WipPen::InternalSCI::get_oK()
{
	return OK;
}

sc_integer FSM_WipPen::InternalSCI::get_nO_LSA()
{
	return NO_LSA;
}

sc_integer FSM_WipPen::InternalSCI::get_sUCCESS()
{
	return SUCCESS;
}

sc_integer FSM_WipPen::InternalSCI::get_fAILURE()
{
	return FAILURE;
}

sc_integer FSM_WipPen::InternalSCI::get_iN_PROGRESS()
{
	return IN_PROGRESS;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_A()
{
	return LSA_A;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_G()
{
	return LSA_G;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_OppG()
{
	return LSA_OppG;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_1()
{
	return LSA_1;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_2()
{
	return LSA_2;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_3()
{
	return LSA_3;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_4()
{
	return LSA_4;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_5()
{
	return LSA_5;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_B()
{
	return LSA_B;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_C()
{
	return LSA_C;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_D()
{
	return LSA_D;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_E()
{
	return LSA_E;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_F()
{
	return LSA_F;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_OppD()
{
	return LSA_OppD;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_OppE()
{
	return LSA_OppE;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_OppF()
{
	return LSA_OppF;
}

sc_integer FSM_WipPen::InternalSCI::get_lSA_6()
{
	return LSA_6;
}


// implementations of all internal functions

sc_boolean FSM_WipPen::check_main_region_StateA_tr0_tr0()
{
	return false && iface_OCB->targetReached();
}

sc_boolean FSM_WipPen::check_main_region_StateA_tr1_tr1()
{
	return (timeEvents[0]) && (false);
}

sc_boolean FSM_WipPen::check_main_region_StateA_tr2_tr2()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_WipPen::check_main_region_StateB_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_WipPen::check_main_region_LSA_tr0_tr0()
{
	return ifaceInternalSCI.SUCCESS == iface_OCB->getLSAStatus();
}

sc_boolean FSM_WipPen::check_main_region_Recal_Front_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_WipPen::check_main_region_Recal_Rear_tr0_tr0()
{
	return iface_OCB->targetReached();
}

void FSM_WipPen::effect_main_region_StateA_tr0()
{
	exseq_main_region_StateA();
	enseq_main_region_StateB_default();
}

void FSM_WipPen::effect_main_region_StateA_tr1()
{
	exseq_main_region_StateA();
	enseq_main_region_StateA_default();
}

void FSM_WipPen::effect_main_region_StateA_tr2()
{
	exseq_main_region_StateA();
	enseq_main_region_Recal_Rear_default();
}

void FSM_WipPen::effect_main_region_StateB_tr0()
{
	exseq_main_region_StateB();
	enseq_main_region_LSA_default();
}

void FSM_WipPen::effect_main_region_LSA_tr0()
{
	exseq_main_region_LSA();
	enseq_main_region_StateA_default();
}

void FSM_WipPen::effect_main_region_Recal_Front_tr0()
{
	exseq_main_region_Recal_Front();
	enseq_main_region_Recal_Rear_default();
}

void FSM_WipPen::effect_main_region_Recal_Rear_tr0()
{
	exseq_main_region_Recal_Rear();
	enseq_main_region_Recal_Front_default();
}

/* Entry action for state 'StateA'. */
void FSM_WipPen::enact_main_region_StateA()
{
	/* Entry action for state 'StateA'. */
	timer->setTimer(this, (sc_eventid)(&timeEvents[0]), 1 * 1000, false);
	iface_OCB->goTo_ID(0, 750, ifaceInternalSCI.FWD);
}

/* Entry action for state 'StateB'. */
void FSM_WipPen::enact_main_region_StateB()
{
	/* Entry action for state 'StateB'. */
	iface_OCB->led4(1);
	iface_OCB->goToLSAEntry(ifaceInternalSCI.LSA_A, ifaceInternalSCI.FWD);
}

/* Entry action for state 'LSA'. */
void FSM_WipPen::enact_main_region_LSA()
{
	/* Entry action for state 'LSA'. */
	iface_OCB->startLSA(ifaceInternalSCI.LSA_A);
}

/* Entry action for state 'Recal Front'. */
void FSM_WipPen::enact_main_region_Recal_Front()
{
	/* Entry action for state 'Recal Front'. */
	iface_OCB->recalFace(ifaceInternalSCI.TOP_Y);
}

/* Entry action for state 'Recal Rear'. */
void FSM_WipPen::enact_main_region_Recal_Rear()
{
	/* Entry action for state 'Recal Rear'. */
	iface_OCB->recalRear(ifaceInternalSCI.TOP_Y);
}

/* Exit action for state 'StateA'. */
void FSM_WipPen::exact_main_region_StateA()
{
	/* Exit action for state 'StateA'. */
	timer->unsetTimer(this, (sc_eventid)(&timeEvents[0]));
}

/* Exit action for state 'StateB'. */
void FSM_WipPen::exact_main_region_StateB()
{
	/* Exit action for state 'StateB'. */
	iface_OCB->led4(0);
}

/* Exit action for state 'LSA'. */
void FSM_WipPen::exact_main_region_LSA()
{
	/* Exit action for state 'LSA'. */
	iface_OCB->stopLSA();
}

/* 'default' enter sequence for state StateA */
void FSM_WipPen::enseq_main_region_StateA_default()
{
	/* 'default' enter sequence for state StateA */
	enact_main_region_StateA();
	stateConfVector[0] = main_region_StateA;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state StateB */
void FSM_WipPen::enseq_main_region_StateB_default()
{
	/* 'default' enter sequence for state StateB */
	enact_main_region_StateB();
	stateConfVector[0] = main_region_StateB;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state LSA */
void FSM_WipPen::enseq_main_region_LSA_default()
{
	/* 'default' enter sequence for state LSA */
	enact_main_region_LSA();
	stateConfVector[0] = main_region_LSA;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Recal Front */
void FSM_WipPen::enseq_main_region_Recal_Front_default()
{
	/* 'default' enter sequence for state Recal Front */
	enact_main_region_Recal_Front();
	stateConfVector[0] = main_region_Recal_Front;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Recal Rear */
void FSM_WipPen::enseq_main_region_Recal_Rear_default()
{
	/* 'default' enter sequence for state Recal Rear */
	enact_main_region_Recal_Rear();
	stateConfVector[0] = main_region_Recal_Rear;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
void FSM_WipPen::enseq_main_region_default()
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default();
}

/* Default exit sequence for state StateA */
void FSM_WipPen::exseq_main_region_StateA()
{
	/* Default exit sequence for state StateA */
	stateConfVector[0] = FSM_WipPen_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_StateA();
}

/* Default exit sequence for state StateB */
void FSM_WipPen::exseq_main_region_StateB()
{
	/* Default exit sequence for state StateB */
	stateConfVector[0] = FSM_WipPen_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_StateB();
}

/* Default exit sequence for state LSA */
void FSM_WipPen::exseq_main_region_LSA()
{
	/* Default exit sequence for state LSA */
	stateConfVector[0] = FSM_WipPen_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_LSA();
}

/* Default exit sequence for state Recal Front */
void FSM_WipPen::exseq_main_region_Recal_Front()
{
	/* Default exit sequence for state Recal Front */
	stateConfVector[0] = FSM_WipPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Recal Rear */
void FSM_WipPen::exseq_main_region_Recal_Rear()
{
	/* Default exit sequence for state Recal Rear */
	stateConfVector[0] = FSM_WipPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
void FSM_WipPen::exseq_main_region()
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of FSM_WipPen.main_region) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case main_region_StateA :
		{
			exseq_main_region_StateA();
			break;
		}
		case main_region_StateB :
		{
			exseq_main_region_StateB();
			break;
		}
		case main_region_LSA :
		{
			exseq_main_region_LSA();
			break;
		}
		case main_region_Recal_Front :
		{
			exseq_main_region_Recal_Front();
			break;
		}
		case main_region_Recal_Rear :
		{
			exseq_main_region_Recal_Rear();
			break;
		}
		default: break;
	}
}

/* The reactions of state StateA. */
void FSM_WipPen::react_main_region_StateA()
{
	/* The reactions of state StateA. */
	if (check_main_region_StateA_tr0_tr0())
	{ 
		effect_main_region_StateA_tr0();
	}  else
	{
		if (check_main_region_StateA_tr1_tr1())
		{ 
			effect_main_region_StateA_tr1();
		}  else
		{
			if (check_main_region_StateA_tr2_tr2())
			{ 
				effect_main_region_StateA_tr2();
			} 
		}
	}
}

/* The reactions of state StateB. */
void FSM_WipPen::react_main_region_StateB()
{
	/* The reactions of state StateB. */
	if (check_main_region_StateB_tr0_tr0())
	{ 
		effect_main_region_StateB_tr0();
	} 
}

/* The reactions of state LSA. */
void FSM_WipPen::react_main_region_LSA()
{
	/* The reactions of state LSA. */
	if (check_main_region_LSA_tr0_tr0())
	{ 
		effect_main_region_LSA_tr0();
	} 
}

/* The reactions of state Recal Front. */
void FSM_WipPen::react_main_region_Recal_Front()
{
	/* The reactions of state Recal Front. */
	if (check_main_region_Recal_Front_tr0_tr0())
	{ 
		effect_main_region_Recal_Front_tr0();
	} 
}

/* The reactions of state Recal Rear. */
void FSM_WipPen::react_main_region_Recal_Rear()
{
	/* The reactions of state Recal Rear. */
	if (check_main_region_Recal_Rear_tr0_tr0())
	{ 
		effect_main_region_Recal_Rear_tr0();
	} 
}

/* Default react sequence for initial entry  */
void FSM_WipPen::react_main_region__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_StateA_default();
}


