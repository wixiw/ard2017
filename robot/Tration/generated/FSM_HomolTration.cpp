
#include "FSM_HomolTration.h"
#include <string.h>

/*! \file Implementation of the state machine 'FSM_HomolTration'
*/

FSM_HomolTration::FSM_HomolTration()
{
	
	iface_OCB = null;
	
	stateConfVectorPosition = 0;
	
	timer = null;
}

FSM_HomolTration::~FSM_HomolTration()
{
}


void FSM_HomolTration::init()
{
	for (int i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = FSM_HomolTration_last_state;
	
	stateConfVectorPosition = 0;

	clearInEvents();
	clearOutEvents();
	
	/* Default init sequence for statechart FSM_HomolTration */
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

void FSM_HomolTration::enter()
{
	/* Default enter sequence for statechart FSM_HomolTration */
	enseq_main_region_default();
}

void FSM_HomolTration::exit()
{
	/* Default exit sequence for statechart FSM_HomolTration */
	exseq_main_region();
}

sc_boolean FSM_HomolTration::isActive()
{
	return stateConfVector[0] != FSM_HomolTration_last_state;
}

sc_boolean FSM_HomolTration::isFinal()
{
	return (stateConfVector[0] == main_region__final_);
}

void FSM_HomolTration::runCycle()
{
	
	clearOutEvents();
	
	for (stateConfVectorPosition = 0;
		stateConfVectorPosition < maxOrthogonalStates;
		stateConfVectorPosition++)
		{
			
		switch (stateConfVector[stateConfVectorPosition])
		{
		case main_region_Escaping_from_Start_area___Goto_Thru_D :
		{
			react_main_region_Escaping_from_Start_area___Goto_Thru_D();
			break;
		}
		case main_region_Go_To_Dispenser_G :
		{
			react_main_region_Go_To_Dispenser_G();
			break;
		}
		case main_region_Goto_Thru_E :
		{
			react_main_region_Goto_Thru_E();
			break;
		}
		case main_region_Get_4_bicolor_cylinders_from_Dispenser_G :
		{
			react_main_region_Get_4_bicolor_cylinders_from_Dispenser_G();
			break;
		}
		case main_region__final_ :
		{
			react_main_region__final_();
			break;
		}
		case main_region_Wait_for_Pen_to_finish_his_move :
		{
			react_main_region_Wait_for_Pen_to_finish_his_move();
			break;
		}
		case main_region_Escape_and_avoid_Pen :
		{
			react_main_region_Escape_and_avoid_Pen();
			break;
		}
		case main_region_Escaping_crater :
		{
			react_main_region_Escaping_crater();
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
		case main_region_Going_in_opponent_side :
		{
			react_main_region_Going_in_opponent_side();
			break;
		}
		case main_region_Going_to_Opp_E :
		{
			react_main_region_Going_to_Opp_E();
			break;
		}
		case main_region_Going_to_Opp_bicolor_Dispenser :
		{
			react_main_region_Going_to_Opp_bicolor_Dispenser();
			break;
		}
		case main_region_Copy_1_Pooing_in_Start_Area :
		{
			react_main_region_Copy_1_Pooing_in_Start_Area();
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents();
}

void FSM_HomolTration::clearInEvents()
{
	timeEvents[0] = false; 
}

void FSM_HomolTration::clearOutEvents()
{
}


void FSM_HomolTration::setTimer(TimerInterface* timer)
{
	this->timer = timer;
}

TimerInterface* FSM_HomolTration::getTimer()
{
	return timer;
}

void FSM_HomolTration::raiseTimeEvent(sc_eventid evid)
{
	if ((evid >= (sc_eventid)timeEvents) && (evid < (sc_eventid)(&timeEvents[timeEventsCount])))
	{
		*(sc_boolean*)evid = true;
	}				
}

sc_boolean FSM_HomolTration::isStateActive(FSM_HomolTrationStates state)
{
	switch (state)
	{
		case main_region_Escaping_from_Start_area___Goto_Thru_D : 
			return (sc_boolean) (stateConfVector[0] == main_region_Escaping_from_Start_area___Goto_Thru_D
			);
		case main_region_Go_To_Dispenser_G : 
			return (sc_boolean) (stateConfVector[0] == main_region_Go_To_Dispenser_G
			);
		case main_region_Goto_Thru_E : 
			return (sc_boolean) (stateConfVector[0] == main_region_Goto_Thru_E
			);
		case main_region_Get_4_bicolor_cylinders_from_Dispenser_G : 
			return (sc_boolean) (stateConfVector[0] == main_region_Get_4_bicolor_cylinders_from_Dispenser_G
			);
		case main_region__final_ : 
			return (sc_boolean) (stateConfVector[0] == main_region__final_
			);
		case main_region_Wait_for_Pen_to_finish_his_move : 
			return (sc_boolean) (stateConfVector[0] == main_region_Wait_for_Pen_to_finish_his_move
			);
		case main_region_Escape_and_avoid_Pen : 
			return (sc_boolean) (stateConfVector[0] == main_region_Escape_and_avoid_Pen
			);
		case main_region_Escaping_crater : 
			return (sc_boolean) (stateConfVector[0] == main_region_Escaping_crater
			);
		case main_region_Go_to_Start_Poo_entry_point : 
			return (sc_boolean) (stateConfVector[0] == main_region_Go_to_Start_Poo_entry_point
			);
		case main_region_Pooing_in_Start_Area : 
			return (sc_boolean) (stateConfVector[0] == main_region_Pooing_in_Start_Area
			);
		case main_region_Going_in_opponent_side : 
			return (sc_boolean) (stateConfVector[0] == main_region_Going_in_opponent_side
			);
		case main_region_Going_to_Opp_E : 
			return (sc_boolean) (stateConfVector[0] == main_region_Going_to_Opp_E
			);
		case main_region_Going_to_Opp_bicolor_Dispenser : 
			return (sc_boolean) (stateConfVector[0] == main_region_Going_to_Opp_bicolor_Dispenser
			);
		case main_region_Copy_1_Pooing_in_Start_Area : 
			return (sc_boolean) (stateConfVector[0] == main_region_Copy_1_Pooing_in_Start_Area
			);
		default: return false;
	}
}

FSM_HomolTration::DefaultSCI* FSM_HomolTration::getDefaultSCI()
{
	return &iface;
}


void FSM_HomolTration::setDefaultSCI_OCB(DefaultSCI_OCB* operationCallback)
{
	iface_OCB = operationCallback;
}

sc_integer FSM_HomolTration::InternalSCI::get_fWD()
{
	return FWD;
}

sc_integer FSM_HomolTration::InternalSCI::get_bWD()
{
	return BWD;
}

sc_integer FSM_HomolTration::InternalSCI::get_tOP_Y()
{
	return TOP_Y;
}

sc_integer FSM_HomolTration::InternalSCI::get_oWN_SIDE_X()
{
	return OWN_SIDE_X;
}

sc_integer FSM_HomolTration::InternalSCI::get_oPP_SIDE_X()
{
	return OPP_SIDE_X;
}

sc_integer FSM_HomolTration::InternalSCI::get_sTART_AREA_Y()
{
	return START_AREA_Y;
}

sc_integer FSM_HomolTration::InternalSCI::get_bOT_Y()
{
	return BOT_Y;
}

sc_integer FSM_HomolTration::InternalSCI::get_sTART_AREA_X()
{
	return START_AREA_X;
}

sc_integer FSM_HomolTration::InternalSCI::get_lED_OFF()
{
	return LED_OFF;
}

sc_integer FSM_HomolTration::InternalSCI::get_lED_ON()
{
	return LED_ON;
}

sc_integer FSM_HomolTration::InternalSCI::get_sLOW_BLINK()
{
	return SLOW_BLINK;
}

sc_integer FSM_HomolTration::InternalSCI::get_fAST_BLINK()
{
	return FAST_BLINK;
}

sc_integer FSM_HomolTration::InternalSCI::get_ledRED()
{
	return ledRED;
}

sc_integer FSM_HomolTration::InternalSCI::get_ledGREEN()
{
	return ledGREEN;
}

sc_integer FSM_HomolTration::InternalSCI::get_ledBLUE()
{
	return ledBLUE;
}

sc_integer FSM_HomolTration::InternalSCI::get_ledYELLOW()
{
	return ledYELLOW;
}

sc_integer FSM_HomolTration::InternalSCI::get_ledCYAN()
{
	return ledCYAN;
}

sc_integer FSM_HomolTration::InternalSCI::get_ledPURPLE()
{
	return ledPURPLE;
}

sc_integer FSM_HomolTration::InternalSCI::get_ledWHITE()
{
	return ledWHITE;
}

sc_integer FSM_HomolTration::InternalSCI::get_cylUNKNOWN()
{
	return cylUNKNOWN;
}

sc_integer FSM_HomolTration::InternalSCI::get_cylYELLOW()
{
	return cylYELLOW;
}

sc_integer FSM_HomolTration::InternalSCI::get_cylBLUE()
{
	return cylBLUE;
}

sc_integer FSM_HomolTration::InternalSCI::get_cylWHITE()
{
	return cylWHITE;
}

sc_integer FSM_HomolTration::InternalSCI::get_nO_CMD()
{
	return NO_CMD;
}

sc_integer FSM_HomolTration::InternalSCI::get_fAILED()
{
	return FAILED;
}

sc_integer FSM_HomolTration::InternalSCI::get_oK()
{
	return OK;
}

sc_integer FSM_HomolTration::InternalSCI::get_nO_LSA()
{
	return NO_LSA;
}

sc_integer FSM_HomolTration::InternalSCI::get_sUCCESS()
{
	return SUCCESS;
}

sc_integer FSM_HomolTration::InternalSCI::get_fAILURE()
{
	return FAILURE;
}

sc_integer FSM_HomolTration::InternalSCI::get_iN_PROGRESS()
{
	return IN_PROGRESS;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_A()
{
	return LSA_A;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_G()
{
	return LSA_G;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_OppG()
{
	return LSA_OppG;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_1()
{
	return LSA_1;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_2()
{
	return LSA_2;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_3()
{
	return LSA_3;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_4()
{
	return LSA_4;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_5()
{
	return LSA_5;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_B()
{
	return LSA_B;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_C()
{
	return LSA_C;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_D()
{
	return LSA_D;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_E()
{
	return LSA_E;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_F()
{
	return LSA_F;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_OppD()
{
	return LSA_OppD;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_OppE()
{
	return LSA_OppE;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_OppF()
{
	return LSA_OppF;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_6()
{
	return LSA_6;
}

sc_integer FSM_HomolTration::InternalSCI::get_lSA_IA()
{
	return LSA_IA;
}


// implementations of all internal functions

sc_boolean FSM_HomolTration::check_main_region_Escaping_from_Start_area___Goto_Thru_D_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolTration::check_main_region_Go_To_Dispenser_G_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolTration::check_main_region_Goto_Thru_E_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolTration::check_main_region_Get_4_bicolor_cylinders_from_Dispenser_G_tr0_tr0()
{
	return ifaceInternalSCI.SUCCESS == iface_OCB->getLSAStatus() || ifaceInternalSCI.FAILURE == iface_OCB->getLSAStatus();
}

sc_boolean FSM_HomolTration::check_main_region_Wait_for_Pen_to_finish_his_move_tr0_tr0()
{
	return timeEvents[0];
}

sc_boolean FSM_HomolTration::check_main_region_Escape_and_avoid_Pen_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolTration::check_main_region_Escaping_crater_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolTration::check_main_region_Go_to_Start_Poo_entry_point_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolTration::check_main_region_Pooing_in_Start_Area_tr0_tr0()
{
	return ifaceInternalSCI.SUCCESS == iface_OCB->getLSAStatus() || ifaceInternalSCI.FAILURE == iface_OCB->getLSAStatus();
}

sc_boolean FSM_HomolTration::check_main_region_Going_in_opponent_side_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolTration::check_main_region_Going_to_Opp_E_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolTration::check_main_region_Going_to_Opp_bicolor_Dispenser_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolTration::check_main_region_Copy_1_Pooing_in_Start_Area_tr0_tr0()
{
	return ifaceInternalSCI.SUCCESS == iface_OCB->getLSAStatus() || ifaceInternalSCI.FAILURE == iface_OCB->getLSAStatus();
}

void FSM_HomolTration::effect_main_region_Escaping_from_Start_area___Goto_Thru_D_tr0()
{
	exseq_main_region_Escaping_from_Start_area___Goto_Thru_D();
	enseq_main_region_Goto_Thru_E_default();
}

void FSM_HomolTration::effect_main_region_Go_To_Dispenser_G_tr0()
{
	exseq_main_region_Go_To_Dispenser_G();
	enseq_main_region_Get_4_bicolor_cylinders_from_Dispenser_G_default();
}

void FSM_HomolTration::effect_main_region_Goto_Thru_E_tr0()
{
	exseq_main_region_Goto_Thru_E();
	enseq_main_region_Go_To_Dispenser_G_default();
}

void FSM_HomolTration::effect_main_region_Get_4_bicolor_cylinders_from_Dispenser_G_tr0()
{
	exseq_main_region_Get_4_bicolor_cylinders_from_Dispenser_G();
	enseq_main_region_Escape_and_avoid_Pen_default();
}

void FSM_HomolTration::effect_main_region_Wait_for_Pen_to_finish_his_move_tr0()
{
	exseq_main_region_Wait_for_Pen_to_finish_his_move();
	enseq_main_region_Escaping_crater_default();
}

void FSM_HomolTration::effect_main_region_Escape_and_avoid_Pen_tr0()
{
	exseq_main_region_Escape_and_avoid_Pen();
	enseq_main_region_Wait_for_Pen_to_finish_his_move_default();
}

void FSM_HomolTration::effect_main_region_Escaping_crater_tr0()
{
	exseq_main_region_Escaping_crater();
	enseq_main_region_Go_to_Start_Poo_entry_point_default();
}

void FSM_HomolTration::effect_main_region_Go_to_Start_Poo_entry_point_tr0()
{
	exseq_main_region_Go_to_Start_Poo_entry_point();
	enseq_main_region_Pooing_in_Start_Area_default();
}

void FSM_HomolTration::effect_main_region_Pooing_in_Start_Area_tr0()
{
	exseq_main_region_Pooing_in_Start_Area();
	enseq_main_region_Going_in_opponent_side_default();
}

void FSM_HomolTration::effect_main_region_Going_in_opponent_side_tr0()
{
	exseq_main_region_Going_in_opponent_side();
	enseq_main_region_Going_to_Opp_E_default();
}

void FSM_HomolTration::effect_main_region_Going_to_Opp_E_tr0()
{
	exseq_main_region_Going_to_Opp_E();
	enseq_main_region_Going_to_Opp_bicolor_Dispenser_default();
}

void FSM_HomolTration::effect_main_region_Going_to_Opp_bicolor_Dispenser_tr0()
{
	exseq_main_region_Going_to_Opp_bicolor_Dispenser();
	enseq_main_region_Copy_1_Pooing_in_Start_Area_default();
}

void FSM_HomolTration::effect_main_region_Copy_1_Pooing_in_Start_Area_tr0()
{
	exseq_main_region_Copy_1_Pooing_in_Start_Area();
	enseq_main_region__final__default();
}

/* Entry action for state 'Escaping from Start area : Goto Thru D'. */
void FSM_HomolTration::enact_main_region_Escaping_from_Start_area___Goto_Thru_D()
{
	/* Entry action for state 'Escaping from Start area : Goto Thru D'. */
	iface_OCB->setPosition(518, 785, -90);
	iface_OCB->goTo_ID(518, 382, ifaceInternalSCI.FWD);
	iface_OCB->logInfo("Moving away from start area so that Pen begin its match...");
}

/* Entry action for state 'Go To Dispenser G'. */
void FSM_HomolTration::enact_main_region_Go_To_Dispenser_G()
{
	/* Entry action for state 'Go To Dispenser G'. */
	iface_OCB->logInfo("Going to dispenser G...");
	iface_OCB->goToLSAEntry(ifaceInternalSCI.LSA_G, ifaceInternalSCI.FWD);
}

/* Entry action for state 'Goto Thru E'. */
void FSM_HomolTration::enact_main_region_Goto_Thru_E()
{
	/* Entry action for state 'Goto Thru E'. */
	iface_OCB->goTo_ID(940, -40, ifaceInternalSCI.FWD);
}

/* Entry action for state 'Get 4 bicolor cylinders from Dispenser G'. */
void FSM_HomolTration::enact_main_region_Get_4_bicolor_cylinders_from_Dispenser_G()
{
	/* Entry action for state 'Get 4 bicolor cylinders from Dispenser G'. */
	iface_OCB->logInfo("Withdrawing elements from dispenser G ...");
	iface_OCB->startLSA(ifaceInternalSCI.LSA_G);
}

/* Entry action for state 'Wait for Pen to finish his move'. */
void FSM_HomolTration::enact_main_region_Wait_for_Pen_to_finish_his_move()
{
	/* Entry action for state 'Wait for Pen to finish his move'. */
	timer->setTimer(this, (sc_eventid)(&timeEvents[0]), 6 * 1000, false);
	iface_OCB->logInfo("Waiting for Pen to arrive...");
}

/* Entry action for state 'Escape and avoid Pen'. */
void FSM_HomolTration::enact_main_region_Escape_and_avoid_Pen()
{
	/* Entry action for state 'Escape and avoid Pen'. */
	iface_OCB->logInfo("Going to poo in start area...");
	iface_OCB->goToCap(1150, 0, -25, ifaceInternalSCI.FWD);
}

/* Entry action for state 'Escaping crater'. */
void FSM_HomolTration::enact_main_region_Escaping_crater()
{
	/* Entry action for state 'Escaping crater'. */
	iface_OCB->goTo_ID(500, 300, ifaceInternalSCI.BWD);
}

/* Entry action for state 'Go to Start Poo entry point'. */
void FSM_HomolTration::enact_main_region_Go_to_Start_Poo_entry_point()
{
	/* Entry action for state 'Go to Start Poo entry point'. */
	iface_OCB->goToLSAEntry(ifaceInternalSCI.LSA_6, ifaceInternalSCI.FWD);
}

/* Entry action for state 'Pooing in Start Area'. */
void FSM_HomolTration::enact_main_region_Pooing_in_Start_Area()
{
	/* Entry action for state 'Pooing in Start Area'. */
	iface_OCB->logInfo("Poing elements in start area ...");
	iface_OCB->startLSA(ifaceInternalSCI.LSA_6);
}

/* Entry action for state 'Going in opponent side'. */
void FSM_HomolTration::enact_main_region_Going_in_opponent_side()
{
	/* Entry action for state 'Going in opponent side'. */
	iface_OCB->goTo_ID(-500, 400, ifaceInternalSCI.FWD);
}

/* Entry action for state 'Going to Opp E'. */
void FSM_HomolTration::enact_main_region_Going_to_Opp_E()
{
	/* Entry action for state 'Going to Opp E'. */
	iface_OCB->goTo_ID(-1000, -100, ifaceInternalSCI.FWD);
}

/* Entry action for state 'Going to Opp bicolor Dispenser'. */
void FSM_HomolTration::enact_main_region_Going_to_Opp_bicolor_Dispenser()
{
	/* Entry action for state 'Going to Opp bicolor Dispenser'. */
	iface_OCB->goToLSAEntry(ifaceInternalSCI.LSA_OppG, ifaceInternalSCI.FWD);
}

/* Entry action for state 'Copy_1_Pooing in Start Area'. */
void FSM_HomolTration::enact_main_region_Copy_1_Pooing_in_Start_Area()
{
	/* Entry action for state 'Copy_1_Pooing in Start Area'. */
	iface_OCB->logInfo("Withdrawing elements from dispenser OppG ...");
	iface_OCB->startLSA(ifaceInternalSCI.LSA_OppG);
}

/* Exit action for state 'Escaping from Start area : Goto Thru D'. */
void FSM_HomolTration::exact_main_region_Escaping_from_Start_area___Goto_Thru_D()
{
	/* Exit action for state 'Escaping from Start area : Goto Thru D'. */
	iface_OCB->logInfo("Start area is freed for PEN.");
	iface_OCB->informTaken_D();
}

/* Exit action for state 'Goto Thru E'. */
void FSM_HomolTration::exact_main_region_Goto_Thru_E()
{
	/* Exit action for state 'Goto Thru E'. */
	iface_OCB->informTaken_E();
}

/* Exit action for state 'Get 4 bicolor cylinders from Dispenser G'. */
void FSM_HomolTration::exact_main_region_Get_4_bicolor_cylinders_from_Dispenser_G()
{
	/* Exit action for state 'Get 4 bicolor cylinders from Dispenser G'. */
	iface_OCB->stopLSA();
}

/* Exit action for state 'Wait for Pen to finish his move'. */
void FSM_HomolTration::exact_main_region_Wait_for_Pen_to_finish_his_move()
{
	/* Exit action for state 'Wait for Pen to finish his move'. */
	timer->unsetTimer(this, (sc_eventid)(&timeEvents[0]));
}

/* Exit action for state 'Pooing in Start Area'. */
void FSM_HomolTration::exact_main_region_Pooing_in_Start_Area()
{
	/* Exit action for state 'Pooing in Start Area'. */
	iface_OCB->stopLSA();
}

/* Exit action for state 'Going in opponent side'. */
void FSM_HomolTration::exact_main_region_Going_in_opponent_side()
{
	/* Exit action for state 'Going in opponent side'. */
	iface_OCB->informTaken_Opp_D();
}

/* Exit action for state 'Going to Opp E'. */
void FSM_HomolTration::exact_main_region_Going_to_Opp_E()
{
	/* Exit action for state 'Going to Opp E'. */
	iface_OCB->informTaken_Opp_E();
}

/* Exit action for state 'Copy_1_Pooing in Start Area'. */
void FSM_HomolTration::exact_main_region_Copy_1_Pooing_in_Start_Area()
{
	/* Exit action for state 'Copy_1_Pooing in Start Area'. */
	iface_OCB->stopLSA();
}

/* 'default' enter sequence for state Escaping from Start area : Goto Thru D */
void FSM_HomolTration::enseq_main_region_Escaping_from_Start_area___Goto_Thru_D_default()
{
	/* 'default' enter sequence for state Escaping from Start area : Goto Thru D */
	enact_main_region_Escaping_from_Start_area___Goto_Thru_D();
	stateConfVector[0] = main_region_Escaping_from_Start_area___Goto_Thru_D;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Go To Dispenser G */
void FSM_HomolTration::enseq_main_region_Go_To_Dispenser_G_default()
{
	/* 'default' enter sequence for state Go To Dispenser G */
	enact_main_region_Go_To_Dispenser_G();
	stateConfVector[0] = main_region_Go_To_Dispenser_G;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Goto Thru E */
void FSM_HomolTration::enseq_main_region_Goto_Thru_E_default()
{
	/* 'default' enter sequence for state Goto Thru E */
	enact_main_region_Goto_Thru_E();
	stateConfVector[0] = main_region_Goto_Thru_E;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Get 4 bicolor cylinders from Dispenser G */
void FSM_HomolTration::enseq_main_region_Get_4_bicolor_cylinders_from_Dispenser_G_default()
{
	/* 'default' enter sequence for state Get 4 bicolor cylinders from Dispenser G */
	enact_main_region_Get_4_bicolor_cylinders_from_Dispenser_G();
	stateConfVector[0] = main_region_Get_4_bicolor_cylinders_from_Dispenser_G;
	stateConfVectorPosition = 0;
}

/* Default enter sequence for state null */
void FSM_HomolTration::enseq_main_region__final__default()
{
	/* Default enter sequence for state null */
	stateConfVector[0] = main_region__final_;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Wait for Pen to finish his move */
void FSM_HomolTration::enseq_main_region_Wait_for_Pen_to_finish_his_move_default()
{
	/* 'default' enter sequence for state Wait for Pen to finish his move */
	enact_main_region_Wait_for_Pen_to_finish_his_move();
	stateConfVector[0] = main_region_Wait_for_Pen_to_finish_his_move;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Escape and avoid Pen */
void FSM_HomolTration::enseq_main_region_Escape_and_avoid_Pen_default()
{
	/* 'default' enter sequence for state Escape and avoid Pen */
	enact_main_region_Escape_and_avoid_Pen();
	stateConfVector[0] = main_region_Escape_and_avoid_Pen;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Escaping crater */
void FSM_HomolTration::enseq_main_region_Escaping_crater_default()
{
	/* 'default' enter sequence for state Escaping crater */
	enact_main_region_Escaping_crater();
	stateConfVector[0] = main_region_Escaping_crater;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Go to Start Poo entry point */
void FSM_HomolTration::enseq_main_region_Go_to_Start_Poo_entry_point_default()
{
	/* 'default' enter sequence for state Go to Start Poo entry point */
	enact_main_region_Go_to_Start_Poo_entry_point();
	stateConfVector[0] = main_region_Go_to_Start_Poo_entry_point;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Pooing in Start Area */
void FSM_HomolTration::enseq_main_region_Pooing_in_Start_Area_default()
{
	/* 'default' enter sequence for state Pooing in Start Area */
	enact_main_region_Pooing_in_Start_Area();
	stateConfVector[0] = main_region_Pooing_in_Start_Area;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Going in opponent side */
void FSM_HomolTration::enseq_main_region_Going_in_opponent_side_default()
{
	/* 'default' enter sequence for state Going in opponent side */
	enact_main_region_Going_in_opponent_side();
	stateConfVector[0] = main_region_Going_in_opponent_side;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Going to Opp E */
void FSM_HomolTration::enseq_main_region_Going_to_Opp_E_default()
{
	/* 'default' enter sequence for state Going to Opp E */
	enact_main_region_Going_to_Opp_E();
	stateConfVector[0] = main_region_Going_to_Opp_E;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Going to Opp bicolor Dispenser */
void FSM_HomolTration::enseq_main_region_Going_to_Opp_bicolor_Dispenser_default()
{
	/* 'default' enter sequence for state Going to Opp bicolor Dispenser */
	enact_main_region_Going_to_Opp_bicolor_Dispenser();
	stateConfVector[0] = main_region_Going_to_Opp_bicolor_Dispenser;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Copy_1_Pooing in Start Area */
void FSM_HomolTration::enseq_main_region_Copy_1_Pooing_in_Start_Area_default()
{
	/* 'default' enter sequence for state Copy_1_Pooing in Start Area */
	enact_main_region_Copy_1_Pooing_in_Start_Area();
	stateConfVector[0] = main_region_Copy_1_Pooing_in_Start_Area;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
void FSM_HomolTration::enseq_main_region_default()
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default();
}

/* Default exit sequence for state Escaping from Start area : Goto Thru D */
void FSM_HomolTration::exseq_main_region_Escaping_from_Start_area___Goto_Thru_D()
{
	/* Default exit sequence for state Escaping from Start area : Goto Thru D */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Escaping_from_Start_area___Goto_Thru_D();
}

/* Default exit sequence for state Go To Dispenser G */
void FSM_HomolTration::exseq_main_region_Go_To_Dispenser_G()
{
	/* Default exit sequence for state Go To Dispenser G */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Goto Thru E */
void FSM_HomolTration::exseq_main_region_Goto_Thru_E()
{
	/* Default exit sequence for state Goto Thru E */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Goto_Thru_E();
}

/* Default exit sequence for state Get 4 bicolor cylinders from Dispenser G */
void FSM_HomolTration::exseq_main_region_Get_4_bicolor_cylinders_from_Dispenser_G()
{
	/* Default exit sequence for state Get 4 bicolor cylinders from Dispenser G */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Get_4_bicolor_cylinders_from_Dispenser_G();
}

/* Default exit sequence for final state. */
void FSM_HomolTration::exseq_main_region__final_()
{
	/* Default exit sequence for final state. */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Wait for Pen to finish his move */
void FSM_HomolTration::exseq_main_region_Wait_for_Pen_to_finish_his_move()
{
	/* Default exit sequence for state Wait for Pen to finish his move */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Wait_for_Pen_to_finish_his_move();
}

/* Default exit sequence for state Escape and avoid Pen */
void FSM_HomolTration::exseq_main_region_Escape_and_avoid_Pen()
{
	/* Default exit sequence for state Escape and avoid Pen */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Escaping crater */
void FSM_HomolTration::exseq_main_region_Escaping_crater()
{
	/* Default exit sequence for state Escaping crater */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Go to Start Poo entry point */
void FSM_HomolTration::exseq_main_region_Go_to_Start_Poo_entry_point()
{
	/* Default exit sequence for state Go to Start Poo entry point */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Pooing in Start Area */
void FSM_HomolTration::exseq_main_region_Pooing_in_Start_Area()
{
	/* Default exit sequence for state Pooing in Start Area */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Pooing_in_Start_Area();
}

/* Default exit sequence for state Going in opponent side */
void FSM_HomolTration::exseq_main_region_Going_in_opponent_side()
{
	/* Default exit sequence for state Going in opponent side */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Going_in_opponent_side();
}

/* Default exit sequence for state Going to Opp E */
void FSM_HomolTration::exseq_main_region_Going_to_Opp_E()
{
	/* Default exit sequence for state Going to Opp E */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Going_to_Opp_E();
}

/* Default exit sequence for state Going to Opp bicolor Dispenser */
void FSM_HomolTration::exseq_main_region_Going_to_Opp_bicolor_Dispenser()
{
	/* Default exit sequence for state Going to Opp bicolor Dispenser */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Copy_1_Pooing in Start Area */
void FSM_HomolTration::exseq_main_region_Copy_1_Pooing_in_Start_Area()
{
	/* Default exit sequence for state Copy_1_Pooing in Start Area */
	stateConfVector[0] = FSM_HomolTration_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Copy_1_Pooing_in_Start_Area();
}

/* Default exit sequence for region main region */
void FSM_HomolTration::exseq_main_region()
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of FSM_HomolTration.main_region) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case main_region_Escaping_from_Start_area___Goto_Thru_D :
		{
			exseq_main_region_Escaping_from_Start_area___Goto_Thru_D();
			break;
		}
		case main_region_Go_To_Dispenser_G :
		{
			exseq_main_region_Go_To_Dispenser_G();
			break;
		}
		case main_region_Goto_Thru_E :
		{
			exseq_main_region_Goto_Thru_E();
			break;
		}
		case main_region_Get_4_bicolor_cylinders_from_Dispenser_G :
		{
			exseq_main_region_Get_4_bicolor_cylinders_from_Dispenser_G();
			break;
		}
		case main_region__final_ :
		{
			exseq_main_region__final_();
			break;
		}
		case main_region_Wait_for_Pen_to_finish_his_move :
		{
			exseq_main_region_Wait_for_Pen_to_finish_his_move();
			break;
		}
		case main_region_Escape_and_avoid_Pen :
		{
			exseq_main_region_Escape_and_avoid_Pen();
			break;
		}
		case main_region_Escaping_crater :
		{
			exseq_main_region_Escaping_crater();
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
		case main_region_Going_in_opponent_side :
		{
			exseq_main_region_Going_in_opponent_side();
			break;
		}
		case main_region_Going_to_Opp_E :
		{
			exseq_main_region_Going_to_Opp_E();
			break;
		}
		case main_region_Going_to_Opp_bicolor_Dispenser :
		{
			exseq_main_region_Going_to_Opp_bicolor_Dispenser();
			break;
		}
		case main_region_Copy_1_Pooing_in_Start_Area :
		{
			exseq_main_region_Copy_1_Pooing_in_Start_Area();
			break;
		}
		default: break;
	}
}

/* The reactions of state Escaping from Start area : Goto Thru D. */
void FSM_HomolTration::react_main_region_Escaping_from_Start_area___Goto_Thru_D()
{
	/* The reactions of state Escaping from Start area : Goto Thru D. */
	if (check_main_region_Escaping_from_Start_area___Goto_Thru_D_tr0_tr0())
	{ 
		effect_main_region_Escaping_from_Start_area___Goto_Thru_D_tr0();
	} 
}

/* The reactions of state Go To Dispenser G. */
void FSM_HomolTration::react_main_region_Go_To_Dispenser_G()
{
	/* The reactions of state Go To Dispenser G. */
	if (check_main_region_Go_To_Dispenser_G_tr0_tr0())
	{ 
		effect_main_region_Go_To_Dispenser_G_tr0();
	} 
}

/* The reactions of state Goto Thru E. */
void FSM_HomolTration::react_main_region_Goto_Thru_E()
{
	/* The reactions of state Goto Thru E. */
	if (check_main_region_Goto_Thru_E_tr0_tr0())
	{ 
		effect_main_region_Goto_Thru_E_tr0();
	} 
}

/* The reactions of state Get 4 bicolor cylinders from Dispenser G. */
void FSM_HomolTration::react_main_region_Get_4_bicolor_cylinders_from_Dispenser_G()
{
	/* The reactions of state Get 4 bicolor cylinders from Dispenser G. */
	if (check_main_region_Get_4_bicolor_cylinders_from_Dispenser_G_tr0_tr0())
	{ 
		effect_main_region_Get_4_bicolor_cylinders_from_Dispenser_G_tr0();
	} 
}

/* The reactions of state null. */
void FSM_HomolTration::react_main_region__final_()
{
	/* The reactions of state null. */
}

/* The reactions of state Wait for Pen to finish his move. */
void FSM_HomolTration::react_main_region_Wait_for_Pen_to_finish_his_move()
{
	/* The reactions of state Wait for Pen to finish his move. */
	if (check_main_region_Wait_for_Pen_to_finish_his_move_tr0_tr0())
	{ 
		effect_main_region_Wait_for_Pen_to_finish_his_move_tr0();
	} 
}

/* The reactions of state Escape and avoid Pen. */
void FSM_HomolTration::react_main_region_Escape_and_avoid_Pen()
{
	/* The reactions of state Escape and avoid Pen. */
	if (check_main_region_Escape_and_avoid_Pen_tr0_tr0())
	{ 
		effect_main_region_Escape_and_avoid_Pen_tr0();
	} 
}

/* The reactions of state Escaping crater. */
void FSM_HomolTration::react_main_region_Escaping_crater()
{
	/* The reactions of state Escaping crater. */
	if (check_main_region_Escaping_crater_tr0_tr0())
	{ 
		effect_main_region_Escaping_crater_tr0();
	} 
}

/* The reactions of state Go to Start Poo entry point. */
void FSM_HomolTration::react_main_region_Go_to_Start_Poo_entry_point()
{
	/* The reactions of state Go to Start Poo entry point. */
	if (check_main_region_Go_to_Start_Poo_entry_point_tr0_tr0())
	{ 
		effect_main_region_Go_to_Start_Poo_entry_point_tr0();
	} 
}

/* The reactions of state Pooing in Start Area. */
void FSM_HomolTration::react_main_region_Pooing_in_Start_Area()
{
	/* The reactions of state Pooing in Start Area. */
	if (check_main_region_Pooing_in_Start_Area_tr0_tr0())
	{ 
		effect_main_region_Pooing_in_Start_Area_tr0();
	} 
}

/* The reactions of state Going in opponent side. */
void FSM_HomolTration::react_main_region_Going_in_opponent_side()
{
	/* The reactions of state Going in opponent side. */
	if (check_main_region_Going_in_opponent_side_tr0_tr0())
	{ 
		effect_main_region_Going_in_opponent_side_tr0();
	} 
}

/* The reactions of state Going to Opp E. */
void FSM_HomolTration::react_main_region_Going_to_Opp_E()
{
	/* The reactions of state Going to Opp E. */
	if (check_main_region_Going_to_Opp_E_tr0_tr0())
	{ 
		effect_main_region_Going_to_Opp_E_tr0();
	} 
}

/* The reactions of state Going to Opp bicolor Dispenser. */
void FSM_HomolTration::react_main_region_Going_to_Opp_bicolor_Dispenser()
{
	/* The reactions of state Going to Opp bicolor Dispenser. */
	if (check_main_region_Going_to_Opp_bicolor_Dispenser_tr0_tr0())
	{ 
		effect_main_region_Going_to_Opp_bicolor_Dispenser_tr0();
	} 
}

/* The reactions of state Copy_1_Pooing in Start Area. */
void FSM_HomolTration::react_main_region_Copy_1_Pooing_in_Start_Area()
{
	/* The reactions of state Copy_1_Pooing in Start Area. */
	if (check_main_region_Copy_1_Pooing_in_Start_Area_tr0_tr0())
	{ 
		effect_main_region_Copy_1_Pooing_in_Start_Area_tr0();
	} 
}

/* Default react sequence for initial entry  */
void FSM_HomolTration::react_main_region__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Escaping_from_Start_area___Goto_Thru_D_default();
}


