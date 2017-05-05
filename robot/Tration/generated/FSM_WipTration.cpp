
#include "FSM_WipTration.h"
#include <string.h>

/*! \file Implementation of the state machine 'FSM_WipTration'
*/

FSM_WipTration::FSM_WipTration()
{
	
	iface_OCB = null;
	
	stateConfVectorPosition = 0;
	
	timer = null;
}

FSM_WipTration::~FSM_WipTration()
{
}


void FSM_WipTration::init()
{
	for (int i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = FSM_WipTration_last_state;
	
	stateConfVectorPosition = 0;

	clearInEvents();
	clearOutEvents();
	
	/* Default init sequence for statechart FSM_WipTration */
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
	ifaceInternalSCI.LSA_IA = 17;
}

void FSM_WipTration::enter()
{
	/* Default enter sequence for statechart FSM_WipTration */
	enseq_main_region_default();
}

void FSM_WipTration::exit()
{
	/* Default exit sequence for statechart FSM_WipTration */
	exseq_main_region();
}

sc_boolean FSM_WipTration::isActive()
{
	return stateConfVector[0] != FSM_WipTration_last_state;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean FSM_WipTration::isFinal()
{
   return false;}

void FSM_WipTration::runCycle()
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
		case main_region_Go_to_Start_Poo_entry_point :
		{
			react_main_region_Go_to_Start_Poo_entry_point();
			break;
		}
		case main_region_Pooing_in_Start_Area :
		{
			react_main_region_Pooing_in_Start_Area();
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents();
}

void FSM_WipTration::clearInEvents()
{
	timeEvents[0] = false; 
}

void FSM_WipTration::clearOutEvents()
{
}


void FSM_WipTration::setTimer(TimerInterface* timer)
{
	this->timer = timer;
}

TimerInterface* FSM_WipTration::getTimer()
{
	return timer;
}

void FSM_WipTration::raiseTimeEvent(sc_eventid evid)
{
	if ((evid >= (sc_eventid)timeEvents) && (evid < (sc_eventid)(&timeEvents[timeEventsCount])))
	{
		*(sc_boolean*)evid = true;
	}				
}

sc_boolean FSM_WipTration::isStateActive(FSM_WipTrationStates state)
{
	switch (state)
	{
		case main_region_StateA : 
			return (sc_boolean) (stateConfVector[0] == main_region_StateA
			);
		case main_region_Go_to_Start_Poo_entry_point : 
			return (sc_boolean) (stateConfVector[0] == main_region_Go_to_Start_Poo_entry_point
			);
		case main_region_Pooing_in_Start_Area : 
			return (sc_boolean) (stateConfVector[0] == main_region_Pooing_in_Start_Area
			);
		default: return false;
	}
}

FSM_WipTration::DefaultSCI* FSM_WipTration::getDefaultSCI()
{
	return &iface;
}


void FSM_WipTration::setDefaultSCI_OCB(DefaultSCI_OCB* operationCallback)
{
	iface_OCB = operationCallback;
}

sc_integer FSM_WipTration::InternalSCI::get_fWD()
{
	return FWD;
}

sc_integer FSM_WipTration::InternalSCI::get_bWD()
{
	return BWD;
}

sc_integer FSM_WipTration::InternalSCI::get_tOP_Y()
{
	return TOP_Y;
}

sc_integer FSM_WipTration::InternalSCI::get_oWN_SIDE_X()
{
	return OWN_SIDE_X;
}

sc_integer FSM_WipTration::InternalSCI::get_oPP_SIDE_X()
{
	return OPP_SIDE_X;
}

sc_integer FSM_WipTration::InternalSCI::get_sTART_AREA_Y()
{
	return START_AREA_Y;
}

sc_integer FSM_WipTration::InternalSCI::get_bOT_Y()
{
	return BOT_Y;
}

sc_integer FSM_WipTration::InternalSCI::get_sTART_AREA_X()
{
	return START_AREA_X;
}

sc_integer FSM_WipTration::InternalSCI::get_lED_OFF()
{
	return LED_OFF;
}

sc_integer FSM_WipTration::InternalSCI::get_lED_ON()
{
	return LED_ON;
}

sc_integer FSM_WipTration::InternalSCI::get_sLOW_BLINK()
{
	return SLOW_BLINK;
}

sc_integer FSM_WipTration::InternalSCI::get_fAST_BLINK()
{
	return FAST_BLINK;
}

sc_integer FSM_WipTration::InternalSCI::get_ledRED()
{
	return ledRED;
}

sc_integer FSM_WipTration::InternalSCI::get_ledGREEN()
{
	return ledGREEN;
}

sc_integer FSM_WipTration::InternalSCI::get_ledBLUE()
{
	return ledBLUE;
}

sc_integer FSM_WipTration::InternalSCI::get_ledYELLOW()
{
	return ledYELLOW;
}

sc_integer FSM_WipTration::InternalSCI::get_ledCYAN()
{
	return ledCYAN;
}

sc_integer FSM_WipTration::InternalSCI::get_ledPURPLE()
{
	return ledPURPLE;
}

sc_integer FSM_WipTration::InternalSCI::get_ledWHITE()
{
	return ledWHITE;
}

sc_integer FSM_WipTration::InternalSCI::get_cylUNKNOWN()
{
	return cylUNKNOWN;
}

sc_integer FSM_WipTration::InternalSCI::get_cylYELLOW()
{
	return cylYELLOW;
}

sc_integer FSM_WipTration::InternalSCI::get_cylBLUE()
{
	return cylBLUE;
}

sc_integer FSM_WipTration::InternalSCI::get_cylWHITE()
{
	return cylWHITE;
}

sc_integer FSM_WipTration::InternalSCI::get_nO_CMD()
{
	return NO_CMD;
}

sc_integer FSM_WipTration::InternalSCI::get_fAILED()
{
	return FAILED;
}

sc_integer FSM_WipTration::InternalSCI::get_oK()
{
	return OK;
}

sc_integer FSM_WipTration::InternalSCI::get_nO_LSA()
{
	return NO_LSA;
}

sc_integer FSM_WipTration::InternalSCI::get_sUCCESS()
{
	return SUCCESS;
}

sc_integer FSM_WipTration::InternalSCI::get_fAILURE()
{
	return FAILURE;
}

sc_integer FSM_WipTration::InternalSCI::get_iN_PROGRESS()
{
	return IN_PROGRESS;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_A()
{
	return LSA_A;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_G()
{
	return LSA_G;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_OppG()
{
	return LSA_OppG;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_1()
{
	return LSA_1;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_2()
{
	return LSA_2;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_3()
{
	return LSA_3;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_4()
{
	return LSA_4;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_5()
{
	return LSA_5;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_B()
{
	return LSA_B;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_C()
{
	return LSA_C;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_D()
{
	return LSA_D;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_E()
{
	return LSA_E;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_F()
{
	return LSA_F;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_OppD()
{
	return LSA_OppD;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_OppE()
{
	return LSA_OppE;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_OppF()
{
	return LSA_OppF;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_6()
{
	return LSA_6;
}

sc_integer FSM_WipTration::InternalSCI::get_lSA_IA()
{
	return LSA_IA;
}


// implementations of all internal functions

sc_boolean FSM_WipTration::check_main_region_StateA_tr0_tr0()
{
	return (timeEvents[0]) && (false);
}

sc_boolean FSM_WipTration::check_main_region_StateA_tr1_tr1()
{
	return true;
}

sc_boolean FSM_WipTration::check_main_region_Go_to_Start_Poo_entry_point_tr0_tr0()
{
	return iface_OCB->targetReached();
}

void FSM_WipTration::effect_main_region_StateA_tr0()
{
	exseq_main_region_StateA();
	enseq_main_region_StateA_default();
}

void FSM_WipTration::effect_main_region_StateA_tr1()
{
	exseq_main_region_StateA();
	enseq_main_region_Go_to_Start_Poo_entry_point_default();
}

void FSM_WipTration::effect_main_region_Go_to_Start_Poo_entry_point_tr0()
{
	exseq_main_region_Go_to_Start_Poo_entry_point();
	enseq_main_region_Pooing_in_Start_Area_default();
}

/* Entry action for state 'StateA'. */
void FSM_WipTration::enact_main_region_StateA()
{
	/* Entry action for state 'StateA'. */
	timer->setTimer(this, (sc_eventid)(&timeEvents[0]), 1 * 1000, false);
	iface_OCB->setPosition(500, 300, 155);
	iface_OCB->informWithdraw_G(4);
	iface_OCB->informWithdraw_OppG(2);
}

/* Entry action for state 'Go to Start Poo entry point'. */
void FSM_WipTration::enact_main_region_Go_to_Start_Poo_entry_point()
{
	/* Entry action for state 'Go to Start Poo entry point'. */
	iface_OCB->goToLSAEntry(ifaceInternalSCI.LSA_6, ifaceInternalSCI.BWD);
}

/* Entry action for state 'Pooing in Start Area'. */
void FSM_WipTration::enact_main_region_Pooing_in_Start_Area()
{
	/* Entry action for state 'Pooing in Start Area'. */
	iface_OCB->logInfo("Poing elements in start area ...");
	iface_OCB->startLSA(ifaceInternalSCI.LSA_6);
}

/* Exit action for state 'StateA'. */
void FSM_WipTration::exact_main_region_StateA()
{
	/* Exit action for state 'StateA'. */
	timer->unsetTimer(this, (sc_eventid)(&timeEvents[0]));
}

/* Exit action for state 'Pooing in Start Area'. */
void FSM_WipTration::exact_main_region_Pooing_in_Start_Area()
{
	/* Exit action for state 'Pooing in Start Area'. */
	iface_OCB->stopLSA();
}

/* 'default' enter sequence for state StateA */
void FSM_WipTration::enseq_main_region_StateA_default()
{
	/* 'default' enter sequence for state StateA */
	enact_main_region_StateA();
	stateConfVector[0] = main_region_StateA;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Go to Start Poo entry point */
void FSM_WipTration::enseq_main_region_Go_to_Start_Poo_entry_point_default()
{
	/* 'default' enter sequence for state Go to Start Poo entry point */
	enact_main_region_Go_to_Start_Poo_entry_point();
	stateConfVector[0] = main_region_Go_to_Start_Poo_entry_point;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Pooing in Start Area */
void FSM_WipTration::enseq_main_region_Pooing_in_Start_Area_default()
{
	/* 'default' enter sequence for state Pooing in Start Area */
	enact_main_region_Pooing_in_Start_Area();
	stateConfVector[0] = main_region_Pooing_in_Start_Area;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
void FSM_WipTration::enseq_main_region_default()
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default();
}

/* Default exit sequence for state StateA */
void FSM_WipTration::exseq_main_region_StateA()
{
	/* Default exit sequence for state StateA */
	stateConfVector[0] = FSM_WipTration_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_StateA();
}

/* Default exit sequence for state Go to Start Poo entry point */
void FSM_WipTration::exseq_main_region_Go_to_Start_Poo_entry_point()
{
	/* Default exit sequence for state Go to Start Poo entry point */
	stateConfVector[0] = FSM_WipTration_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Pooing in Start Area */
void FSM_WipTration::exseq_main_region_Pooing_in_Start_Area()
{
	/* Default exit sequence for state Pooing in Start Area */
	stateConfVector[0] = FSM_WipTration_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Pooing_in_Start_Area();
}

/* Default exit sequence for region main region */
void FSM_WipTration::exseq_main_region()
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of FSM_WipTration.main_region) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case main_region_StateA :
		{
			exseq_main_region_StateA();
			break;
		}
		case main_region_Go_to_Start_Poo_entry_point :
		{
			exseq_main_region_Go_to_Start_Poo_entry_point();
			break;
		}
		case main_region_Pooing_in_Start_Area :
		{
			exseq_main_region_Pooing_in_Start_Area();
			break;
		}
		default: break;
	}
}

/* The reactions of state StateA. */
void FSM_WipTration::react_main_region_StateA()
{
	/* The reactions of state StateA. */
	if (check_main_region_StateA_tr0_tr0())
	{ 
		effect_main_region_StateA_tr0();
	}  else
	{
		effect_main_region_StateA_tr1();
	}
}

/* The reactions of state Go to Start Poo entry point. */
void FSM_WipTration::react_main_region_Go_to_Start_Poo_entry_point()
{
	/* The reactions of state Go to Start Poo entry point. */
	if (check_main_region_Go_to_Start_Poo_entry_point_tr0_tr0())
	{ 
		effect_main_region_Go_to_Start_Poo_entry_point_tr0();
	} 
}

/* The reactions of state Pooing in Start Area. */
void FSM_WipTration::react_main_region_Pooing_in_Start_Area()
{
	/* The reactions of state Pooing in Start Area. */
}

/* Default react sequence for initial entry  */
void FSM_WipTration::react_main_region__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_StateA_default();
}

