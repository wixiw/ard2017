
#include "FSM_HomolPen.h"
#include <string.h>

/*! \file Implementation of the state machine 'FSM_HomolPen'
*/

FSM_HomolPen::FSM_HomolPen()
{
	
	iface_OCB = null;
	
	stateConfVectorPosition = 0;
	
	timer = null;
}

FSM_HomolPen::~FSM_HomolPen()
{
}


void FSM_HomolPen::init()
{
	for (int i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = FSM_HomolPen_last_state;
	
	stateConfVectorPosition = 0;

	clearInEvents();
	clearOutEvents();
	
	/* Default init sequence for statechart FSM_HomolPen */
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

void FSM_HomolPen::enter()
{
	/* Default enter sequence for statechart FSM_HomolPen */
	enseq_main_region_default();
}

void FSM_HomolPen::exit()
{
	/* Default exit sequence for statechart FSM_HomolPen */
	exseq_main_region();
}

sc_boolean FSM_HomolPen::isActive()
{
	return stateConfVector[0] != FSM_HomolPen_last_state;
}

sc_boolean FSM_HomolPen::isFinal()
{
	return (stateConfVector[0] == main_region__final_);
}

void FSM_HomolPen::runCycle()
{
	
	clearOutEvents();
	
	for (stateConfVectorPosition = 0;
		stateConfVectorPosition < maxOrthogonalStates;
		stateConfVectorPosition++)
		{
			
		switch (stateConfVector[stateConfVectorPosition])
		{
		case main_region_Go_To_Dispenser_A :
		{
			react_main_region_Go_To_Dispenser_A();
			break;
		}
		case main_region_Go_To_Container_4 :
		{
			react_main_region_Go_To_Container_4();
			break;
		}
		case main_region_Get_4_monocolor_cylinders_from_Dispenser_A :
		{
			react_main_region_Get_4_monocolor_cylinders_from_Dispenser_A();
			break;
		}
		case main_region_Poo_4_monocolor_cylinders_in_4 :
		{
			react_main_region_Poo_4_monocolor_cylinders_in_4();
			break;
		}
		case main_region_Wait_Tration_to_move :
		{
			react_main_region_Wait_Tration_to_move();
			break;
		}
		case main_region_Return_to_Start_Area_to_recal :
		{
			react_main_region_Return_to_Start_Area_to_recal();
			break;
		}
		case main_region_Recal_X_against_Start_Area_flip_flop :
		{
			react_main_region_Recal_X_against_Start_Area_flip_flop();
			break;
		}
		case main_region_Recal_Y_against_Start_Area_border :
		{
			react_main_region_Recal_Y_against_Start_Area_border();
			break;
		}
		case main_region_Get_C_Cylinder :
		{
			react_main_region_Get_C_Cylinder();
			break;
		}
		case main_region_Wait_for_Tration_to_finish_his_move :
		{
			react_main_region_Wait_for_Tration_to_finish_his_move();
			break;
		}
		case main_region_Go_To_Cylinder_C :
		{
			react_main_region_Go_To_Cylinder_C();
			break;
		}
		case main_region__final_ :
		{
			react_main_region__final_();
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents();
}

void FSM_HomolPen::clearInEvents()
{
	timeEvents[0] = false; 
	timeEvents[1] = false; 
}

void FSM_HomolPen::clearOutEvents()
{
}


void FSM_HomolPen::setTimer(TimerInterface* timer)
{
	this->timer = timer;
}

TimerInterface* FSM_HomolPen::getTimer()
{
	return timer;
}

void FSM_HomolPen::raiseTimeEvent(sc_eventid evid)
{
	if ((evid >= (sc_eventid)timeEvents) && (evid < (sc_eventid)(&timeEvents[timeEventsCount])))
	{
		*(sc_boolean*)evid = true;
	}				
}

sc_boolean FSM_HomolPen::isStateActive(FSM_HomolPenStates state)
{
	switch (state)
	{
		case main_region_Go_To_Dispenser_A : 
			return (sc_boolean) (stateConfVector[0] == main_region_Go_To_Dispenser_A
			);
		case main_region_Go_To_Container_4 : 
			return (sc_boolean) (stateConfVector[0] == main_region_Go_To_Container_4
			);
		case main_region_Get_4_monocolor_cylinders_from_Dispenser_A : 
			return (sc_boolean) (stateConfVector[0] == main_region_Get_4_monocolor_cylinders_from_Dispenser_A
			);
		case main_region_Poo_4_monocolor_cylinders_in_4 : 
			return (sc_boolean) (stateConfVector[0] == main_region_Poo_4_monocolor_cylinders_in_4
			);
		case main_region_Wait_Tration_to_move : 
			return (sc_boolean) (stateConfVector[0] == main_region_Wait_Tration_to_move
			);
		case main_region_Return_to_Start_Area_to_recal : 
			return (sc_boolean) (stateConfVector[0] == main_region_Return_to_Start_Area_to_recal
			);
		case main_region_Recal_X_against_Start_Area_flip_flop : 
			return (sc_boolean) (stateConfVector[0] == main_region_Recal_X_against_Start_Area_flip_flop
			);
		case main_region_Recal_Y_against_Start_Area_border : 
			return (sc_boolean) (stateConfVector[0] == main_region_Recal_Y_against_Start_Area_border
			);
		case main_region_Get_C_Cylinder : 
			return (sc_boolean) (stateConfVector[0] == main_region_Get_C_Cylinder
			);
		case main_region_Wait_for_Tration_to_finish_his_move : 
			return (sc_boolean) (stateConfVector[0] == main_region_Wait_for_Tration_to_finish_his_move
			);
		case main_region_Go_To_Cylinder_C : 
			return (sc_boolean) (stateConfVector[0] == main_region_Go_To_Cylinder_C
			);
		case main_region__final_ : 
			return (sc_boolean) (stateConfVector[0] == main_region__final_
			);
		default: return false;
	}
}

FSM_HomolPen::DefaultSCI* FSM_HomolPen::getDefaultSCI()
{
	return &iface;
}


void FSM_HomolPen::setDefaultSCI_OCB(DefaultSCI_OCB* operationCallback)
{
	iface_OCB = operationCallback;
}

sc_integer FSM_HomolPen::InternalSCI::get_fWD()
{
	return FWD;
}

sc_integer FSM_HomolPen::InternalSCI::get_bWD()
{
	return BWD;
}

sc_integer FSM_HomolPen::InternalSCI::get_tOP_Y()
{
	return TOP_Y;
}

sc_integer FSM_HomolPen::InternalSCI::get_oWN_SIDE_X()
{
	return OWN_SIDE_X;
}

sc_integer FSM_HomolPen::InternalSCI::get_oPP_SIDE_X()
{
	return OPP_SIDE_X;
}

sc_integer FSM_HomolPen::InternalSCI::get_sTART_AREA_Y()
{
	return START_AREA_Y;
}

sc_integer FSM_HomolPen::InternalSCI::get_bOT_Y()
{
	return BOT_Y;
}

sc_integer FSM_HomolPen::InternalSCI::get_sTART_AREA_X()
{
	return START_AREA_X;
}

sc_integer FSM_HomolPen::InternalSCI::get_lED_OFF()
{
	return LED_OFF;
}

sc_integer FSM_HomolPen::InternalSCI::get_lED_ON()
{
	return LED_ON;
}

sc_integer FSM_HomolPen::InternalSCI::get_sLOW_BLINK()
{
	return SLOW_BLINK;
}

sc_integer FSM_HomolPen::InternalSCI::get_fAST_BLINK()
{
	return FAST_BLINK;
}

sc_integer FSM_HomolPen::InternalSCI::get_ledRED()
{
	return ledRED;
}

sc_integer FSM_HomolPen::InternalSCI::get_ledGREEN()
{
	return ledGREEN;
}

sc_integer FSM_HomolPen::InternalSCI::get_ledBLUE()
{
	return ledBLUE;
}

sc_integer FSM_HomolPen::InternalSCI::get_ledYELLOW()
{
	return ledYELLOW;
}

sc_integer FSM_HomolPen::InternalSCI::get_ledCYAN()
{
	return ledCYAN;
}

sc_integer FSM_HomolPen::InternalSCI::get_ledPURPLE()
{
	return ledPURPLE;
}

sc_integer FSM_HomolPen::InternalSCI::get_ledWHITE()
{
	return ledWHITE;
}

sc_integer FSM_HomolPen::InternalSCI::get_cylUNKNOWN()
{
	return cylUNKNOWN;
}

sc_integer FSM_HomolPen::InternalSCI::get_cylYELLOW()
{
	return cylYELLOW;
}

sc_integer FSM_HomolPen::InternalSCI::get_cylBLUE()
{
	return cylBLUE;
}

sc_integer FSM_HomolPen::InternalSCI::get_cylWHITE()
{
	return cylWHITE;
}

sc_integer FSM_HomolPen::InternalSCI::get_nO_CMD()
{
	return NO_CMD;
}

sc_integer FSM_HomolPen::InternalSCI::get_fAILED()
{
	return FAILED;
}

sc_integer FSM_HomolPen::InternalSCI::get_oK()
{
	return OK;
}

sc_integer FSM_HomolPen::InternalSCI::get_nO_LSA()
{
	return NO_LSA;
}

sc_integer FSM_HomolPen::InternalSCI::get_sUCCESS()
{
	return SUCCESS;
}

sc_integer FSM_HomolPen::InternalSCI::get_fAILURE()
{
	return FAILURE;
}

sc_integer FSM_HomolPen::InternalSCI::get_iN_PROGRESS()
{
	return IN_PROGRESS;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_A()
{
	return LSA_A;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_G()
{
	return LSA_G;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_OppG()
{
	return LSA_OppG;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_1()
{
	return LSA_1;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_2()
{
	return LSA_2;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_3()
{
	return LSA_3;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_4()
{
	return LSA_4;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_5()
{
	return LSA_5;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_B()
{
	return LSA_B;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_C()
{
	return LSA_C;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_D()
{
	return LSA_D;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_E()
{
	return LSA_E;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_F()
{
	return LSA_F;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_OppD()
{
	return LSA_OppD;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_OppE()
{
	return LSA_OppE;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_OppF()
{
	return LSA_OppF;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_6()
{
	return LSA_6;
}

sc_integer FSM_HomolPen::InternalSCI::get_lSA_IA()
{
	return LSA_IA;
}


// implementations of all internal functions

sc_boolean FSM_HomolPen::check_main_region_Go_To_Dispenser_A_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolPen::check_main_region_Go_To_Container_4_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolPen::check_main_region_Get_4_monocolor_cylinders_from_Dispenser_A_tr0_tr0()
{
	return ifaceInternalSCI.SUCCESS == iface_OCB->getLSAStatus() || ifaceInternalSCI.FAILURE == iface_OCB->getLSAStatus();
}

sc_boolean FSM_HomolPen::check_main_region_Poo_4_monocolor_cylinders_in_4_tr0_tr0()
{
	return ifaceInternalSCI.SUCCESS == iface_OCB->getLSAStatus() || ifaceInternalSCI.FAILURE == iface_OCB->getLSAStatus();
}

sc_boolean FSM_HomolPen::check_main_region_Wait_Tration_to_move_tr0_tr0()
{
	return timeEvents[0];
}

sc_boolean FSM_HomolPen::check_main_region_Return_to_Start_Area_to_recal_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolPen::check_main_region_Recal_X_against_Start_Area_flip_flop_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolPen::check_main_region_Recal_Y_against_Start_Area_border_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_HomolPen::check_main_region_Get_C_Cylinder_tr0_tr0()
{
	return ifaceInternalSCI.SUCCESS == iface_OCB->getLSAStatus() || ifaceInternalSCI.FAILURE == iface_OCB->getLSAStatus();
}

sc_boolean FSM_HomolPen::check_main_region_Wait_for_Tration_to_finish_his_move_tr0_tr0()
{
	return timeEvents[1];
}

sc_boolean FSM_HomolPen::check_main_region_Go_To_Cylinder_C_tr0_tr0()
{
	return iface_OCB->targetReached();
}

void FSM_HomolPen::effect_main_region_Go_To_Dispenser_A_tr0()
{
	exseq_main_region_Go_To_Dispenser_A();
	enseq_main_region_Get_4_monocolor_cylinders_from_Dispenser_A_default();
}

void FSM_HomolPen::effect_main_region_Go_To_Container_4_tr0()
{
	exseq_main_region_Go_To_Container_4();
	enseq_main_region_Poo_4_monocolor_cylinders_in_4_default();
}

void FSM_HomolPen::effect_main_region_Get_4_monocolor_cylinders_from_Dispenser_A_tr0()
{
	exseq_main_region_Get_4_monocolor_cylinders_from_Dispenser_A();
	enseq_main_region_Return_to_Start_Area_to_recal_default();
}

void FSM_HomolPen::effect_main_region_Poo_4_monocolor_cylinders_in_4_tr0()
{
	exseq_main_region_Poo_4_monocolor_cylinders_in_4();
	enseq_main_region_Go_To_Cylinder_C_default();
}

void FSM_HomolPen::effect_main_region_Wait_Tration_to_move_tr0()
{
	exseq_main_region_Wait_Tration_to_move();
	enseq_main_region_Go_To_Dispenser_A_default();
}

void FSM_HomolPen::effect_main_region_Return_to_Start_Area_to_recal_tr0()
{
	exseq_main_region_Return_to_Start_Area_to_recal();
	enseq_main_region_Recal_X_against_Start_Area_flip_flop_default();
}

void FSM_HomolPen::effect_main_region_Recal_X_against_Start_Area_flip_flop_tr0()
{
	exseq_main_region_Recal_X_against_Start_Area_flip_flop();
	enseq_main_region_Recal_Y_against_Start_Area_border_default();
}

void FSM_HomolPen::effect_main_region_Recal_Y_against_Start_Area_border_tr0()
{
	exseq_main_region_Recal_Y_against_Start_Area_border();
	enseq_main_region_Go_To_Container_4_default();
}

void FSM_HomolPen::effect_main_region_Get_C_Cylinder_tr0()
{
	exseq_main_region_Get_C_Cylinder();
	enseq_main_region_Wait_for_Tration_to_finish_his_move_default();
}

void FSM_HomolPen::effect_main_region_Wait_for_Tration_to_finish_his_move_tr0()
{
	exseq_main_region_Wait_for_Tration_to_finish_his_move();
	iface_OCB->setStatus(ifaceInternalSCI.SUCCESS);
	enseq_main_region__final__default();
}

void FSM_HomolPen::effect_main_region_Go_To_Cylinder_C_tr0()
{
	exseq_main_region_Go_To_Cylinder_C();
	enseq_main_region_Get_C_Cylinder_default();
}

/* Entry action for state 'Go To Dispenser A'. */
void FSM_HomolPen::enact_main_region_Go_To_Dispenser_A()
{
	/* Entry action for state 'Go To Dispenser A'. */
	iface_OCB->logInfo("Going to dispenser A...");
	iface_OCB->goToCap(350, 730, 90, ifaceInternalSCI.BWD);
}

/* Entry action for state 'Go To Container 4'. */
void FSM_HomolPen::enact_main_region_Go_To_Container_4()
{
	/* Entry action for state 'Go To Container 4'. */
	iface_OCB->goToLSAEntry(ifaceInternalSCI.LSA_4, ifaceInternalSCI.BWD);
}

/* Entry action for state 'Get 4 monocolor cylinders from Dispenser A'. */
void FSM_HomolPen::enact_main_region_Get_4_monocolor_cylinders_from_Dispenser_A()
{
	/* Entry action for state 'Get 4 monocolor cylinders from Dispenser A'. */
	iface_OCB->logInfo("Withdrawing elements from dispenser A ...");
	iface_OCB->startLSA(ifaceInternalSCI.LSA_A);
}

/* Entry action for state 'Poo 4 monocolor cylinders in 4'. */
void FSM_HomolPen::enact_main_region_Poo_4_monocolor_cylinders_in_4()
{
	/* Entry action for state 'Poo 4 monocolor cylinders in 4'. */
	iface_OCB->logInfo("Pooing 4 cylinders in 4 area.");
	iface_OCB->startLSA(ifaceInternalSCI.LSA_4);
}

/* Entry action for state 'Wait Tration to move'. */
void FSM_HomolPen::enact_main_region_Wait_Tration_to_move()
{
	/* Entry action for state 'Wait Tration to move'. */
	timer->setTimer(this, (sc_eventid)(&timeEvents[0]), 1 * 1000, false);
	iface_OCB->setPosition(640, 730, 0);
	iface_OCB->logInfo("Waiting for Tration to move away...");
}

/* Entry action for state 'Return to Start Area to recal'. */
void FSM_HomolPen::enact_main_region_Return_to_Start_Area_to_recal()
{
	/* Entry action for state 'Return to Start Area to recal'. */
	iface_OCB->goTo_ID(600, 780, ifaceInternalSCI.BWD);
}

/* Entry action for state 'Recal X against Start Area flip-flop'. */
void FSM_HomolPen::enact_main_region_Recal_X_against_Start_Area_flip_flop()
{
	/* Entry action for state 'Recal X against Start Area flip-flop'. */
	iface_OCB->recalRear(ifaceInternalSCI.START_AREA_X);
}

/* Entry action for state 'Recal Y against Start Area border'. */
void FSM_HomolPen::enact_main_region_Recal_Y_against_Start_Area_border()
{
	/* Entry action for state 'Recal Y against Start Area border'. */
	iface_OCB->recalFace(ifaceInternalSCI.TOP_Y);
}

/* Entry action for state 'Get C Cylinder'. */
void FSM_HomolPen::enact_main_region_Get_C_Cylinder()
{
	/* Entry action for state 'Get C Cylinder'. */
	iface_OCB->startLSA(ifaceInternalSCI.LSA_C);
}

/* Entry action for state 'Wait for Tration to finish his move'. */
void FSM_HomolPen::enact_main_region_Wait_for_Tration_to_finish_his_move()
{
	/* Entry action for state 'Wait for Tration to finish his move'. */
	timer->setTimer(this, (sc_eventid)(&timeEvents[1]), 1 * 1000, false);
	iface_OCB->logInfo("Waiting for Tration to move away.");
}

/* Entry action for state 'Go To Cylinder C'. */
void FSM_HomolPen::enact_main_region_Go_To_Cylinder_C()
{
	/* Entry action for state 'Go To Cylinder C'. */
	iface_OCB->goToLSAEntry(ifaceInternalSCI.LSA_C, ifaceInternalSCI.FWD);
}

/* Exit action for state 'Get 4 monocolor cylinders from Dispenser A'. */
void FSM_HomolPen::exact_main_region_Get_4_monocolor_cylinders_from_Dispenser_A()
{
	/* Exit action for state 'Get 4 monocolor cylinders from Dispenser A'. */
	iface_OCB->stopLSA();
}

/* Exit action for state 'Poo 4 monocolor cylinders in 4'. */
void FSM_HomolPen::exact_main_region_Poo_4_monocolor_cylinders_in_4()
{
	/* Exit action for state 'Poo 4 monocolor cylinders in 4'. */
	iface_OCB->stopLSA();
}

/* Exit action for state 'Wait Tration to move'. */
void FSM_HomolPen::exact_main_region_Wait_Tration_to_move()
{
	/* Exit action for state 'Wait Tration to move'. */
	timer->unsetTimer(this, (sc_eventid)(&timeEvents[0]));
	iface_OCB->logInfo("Go go go !");
}

/* Exit action for state 'Get C Cylinder'. */
void FSM_HomolPen::exact_main_region_Get_C_Cylinder()
{
	/* Exit action for state 'Get C Cylinder'. */
	iface_OCB->stopLSA();
}

/* Exit action for state 'Wait for Tration to finish his move'. */
void FSM_HomolPen::exact_main_region_Wait_for_Tration_to_finish_his_move()
{
	/* Exit action for state 'Wait for Tration to finish his move'. */
	timer->unsetTimer(this, (sc_eventid)(&timeEvents[1]));
}

/* 'default' enter sequence for state Go To Dispenser A */
void FSM_HomolPen::enseq_main_region_Go_To_Dispenser_A_default()
{
	/* 'default' enter sequence for state Go To Dispenser A */
	enact_main_region_Go_To_Dispenser_A();
	stateConfVector[0] = main_region_Go_To_Dispenser_A;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Go To Container 4 */
void FSM_HomolPen::enseq_main_region_Go_To_Container_4_default()
{
	/* 'default' enter sequence for state Go To Container 4 */
	enact_main_region_Go_To_Container_4();
	stateConfVector[0] = main_region_Go_To_Container_4;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Get 4 monocolor cylinders from Dispenser A */
void FSM_HomolPen::enseq_main_region_Get_4_monocolor_cylinders_from_Dispenser_A_default()
{
	/* 'default' enter sequence for state Get 4 monocolor cylinders from Dispenser A */
	enact_main_region_Get_4_monocolor_cylinders_from_Dispenser_A();
	stateConfVector[0] = main_region_Get_4_monocolor_cylinders_from_Dispenser_A;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Poo 4 monocolor cylinders in 4 */
void FSM_HomolPen::enseq_main_region_Poo_4_monocolor_cylinders_in_4_default()
{
	/* 'default' enter sequence for state Poo 4 monocolor cylinders in 4 */
	enact_main_region_Poo_4_monocolor_cylinders_in_4();
	stateConfVector[0] = main_region_Poo_4_monocolor_cylinders_in_4;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Wait Tration to move */
void FSM_HomolPen::enseq_main_region_Wait_Tration_to_move_default()
{
	/* 'default' enter sequence for state Wait Tration to move */
	enact_main_region_Wait_Tration_to_move();
	stateConfVector[0] = main_region_Wait_Tration_to_move;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Return to Start Area to recal */
void FSM_HomolPen::enseq_main_region_Return_to_Start_Area_to_recal_default()
{
	/* 'default' enter sequence for state Return to Start Area to recal */
	enact_main_region_Return_to_Start_Area_to_recal();
	stateConfVector[0] = main_region_Return_to_Start_Area_to_recal;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Recal X against Start Area flip-flop */
void FSM_HomolPen::enseq_main_region_Recal_X_against_Start_Area_flip_flop_default()
{
	/* 'default' enter sequence for state Recal X against Start Area flip-flop */
	enact_main_region_Recal_X_against_Start_Area_flip_flop();
	stateConfVector[0] = main_region_Recal_X_against_Start_Area_flip_flop;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Recal Y against Start Area border */
void FSM_HomolPen::enseq_main_region_Recal_Y_against_Start_Area_border_default()
{
	/* 'default' enter sequence for state Recal Y against Start Area border */
	enact_main_region_Recal_Y_against_Start_Area_border();
	stateConfVector[0] = main_region_Recal_Y_against_Start_Area_border;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Get C Cylinder */
void FSM_HomolPen::enseq_main_region_Get_C_Cylinder_default()
{
	/* 'default' enter sequence for state Get C Cylinder */
	enact_main_region_Get_C_Cylinder();
	stateConfVector[0] = main_region_Get_C_Cylinder;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Wait for Tration to finish his move */
void FSM_HomolPen::enseq_main_region_Wait_for_Tration_to_finish_his_move_default()
{
	/* 'default' enter sequence for state Wait for Tration to finish his move */
	enact_main_region_Wait_for_Tration_to_finish_his_move();
	stateConfVector[0] = main_region_Wait_for_Tration_to_finish_his_move;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Go To Cylinder C */
void FSM_HomolPen::enseq_main_region_Go_To_Cylinder_C_default()
{
	/* 'default' enter sequence for state Go To Cylinder C */
	enact_main_region_Go_To_Cylinder_C();
	stateConfVector[0] = main_region_Go_To_Cylinder_C;
	stateConfVectorPosition = 0;
}

/* Default enter sequence for state null */
void FSM_HomolPen::enseq_main_region__final__default()
{
	/* Default enter sequence for state null */
	stateConfVector[0] = main_region__final_;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
void FSM_HomolPen::enseq_main_region_default()
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default();
}

/* Default exit sequence for state Go To Dispenser A */
void FSM_HomolPen::exseq_main_region_Go_To_Dispenser_A()
{
	/* Default exit sequence for state Go To Dispenser A */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Go To Container 4 */
void FSM_HomolPen::exseq_main_region_Go_To_Container_4()
{
	/* Default exit sequence for state Go To Container 4 */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Get 4 monocolor cylinders from Dispenser A */
void FSM_HomolPen::exseq_main_region_Get_4_monocolor_cylinders_from_Dispenser_A()
{
	/* Default exit sequence for state Get 4 monocolor cylinders from Dispenser A */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Get_4_monocolor_cylinders_from_Dispenser_A();
}

/* Default exit sequence for state Poo 4 monocolor cylinders in 4 */
void FSM_HomolPen::exseq_main_region_Poo_4_monocolor_cylinders_in_4()
{
	/* Default exit sequence for state Poo 4 monocolor cylinders in 4 */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Poo_4_monocolor_cylinders_in_4();
}

/* Default exit sequence for state Wait Tration to move */
void FSM_HomolPen::exseq_main_region_Wait_Tration_to_move()
{
	/* Default exit sequence for state Wait Tration to move */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Wait_Tration_to_move();
}

/* Default exit sequence for state Return to Start Area to recal */
void FSM_HomolPen::exseq_main_region_Return_to_Start_Area_to_recal()
{
	/* Default exit sequence for state Return to Start Area to recal */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Recal X against Start Area flip-flop */
void FSM_HomolPen::exseq_main_region_Recal_X_against_Start_Area_flip_flop()
{
	/* Default exit sequence for state Recal X against Start Area flip-flop */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Recal Y against Start Area border */
void FSM_HomolPen::exseq_main_region_Recal_Y_against_Start_Area_border()
{
	/* Default exit sequence for state Recal Y against Start Area border */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Get C Cylinder */
void FSM_HomolPen::exseq_main_region_Get_C_Cylinder()
{
	/* Default exit sequence for state Get C Cylinder */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Get_C_Cylinder();
}

/* Default exit sequence for state Wait for Tration to finish his move */
void FSM_HomolPen::exseq_main_region_Wait_for_Tration_to_finish_his_move()
{
	/* Default exit sequence for state Wait for Tration to finish his move */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Wait_for_Tration_to_finish_his_move();
}

/* Default exit sequence for state Go To Cylinder C */
void FSM_HomolPen::exseq_main_region_Go_To_Cylinder_C()
{
	/* Default exit sequence for state Go To Cylinder C */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for final state. */
void FSM_HomolPen::exseq_main_region__final_()
{
	/* Default exit sequence for final state. */
	stateConfVector[0] = FSM_HomolPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
void FSM_HomolPen::exseq_main_region()
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of FSM_HomolPen.main_region) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case main_region_Go_To_Dispenser_A :
		{
			exseq_main_region_Go_To_Dispenser_A();
			break;
		}
		case main_region_Go_To_Container_4 :
		{
			exseq_main_region_Go_To_Container_4();
			break;
		}
		case main_region_Get_4_monocolor_cylinders_from_Dispenser_A :
		{
			exseq_main_region_Get_4_monocolor_cylinders_from_Dispenser_A();
			break;
		}
		case main_region_Poo_4_monocolor_cylinders_in_4 :
		{
			exseq_main_region_Poo_4_monocolor_cylinders_in_4();
			break;
		}
		case main_region_Wait_Tration_to_move :
		{
			exseq_main_region_Wait_Tration_to_move();
			break;
		}
		case main_region_Return_to_Start_Area_to_recal :
		{
			exseq_main_region_Return_to_Start_Area_to_recal();
			break;
		}
		case main_region_Recal_X_against_Start_Area_flip_flop :
		{
			exseq_main_region_Recal_X_against_Start_Area_flip_flop();
			break;
		}
		case main_region_Recal_Y_against_Start_Area_border :
		{
			exseq_main_region_Recal_Y_against_Start_Area_border();
			break;
		}
		case main_region_Get_C_Cylinder :
		{
			exseq_main_region_Get_C_Cylinder();
			break;
		}
		case main_region_Wait_for_Tration_to_finish_his_move :
		{
			exseq_main_region_Wait_for_Tration_to_finish_his_move();
			break;
		}
		case main_region_Go_To_Cylinder_C :
		{
			exseq_main_region_Go_To_Cylinder_C();
			break;
		}
		case main_region__final_ :
		{
			exseq_main_region__final_();
			break;
		}
		default: break;
	}
}

/* The reactions of state Go To Dispenser A. */
void FSM_HomolPen::react_main_region_Go_To_Dispenser_A()
{
	/* The reactions of state Go To Dispenser A. */
	if (check_main_region_Go_To_Dispenser_A_tr0_tr0())
	{ 
		effect_main_region_Go_To_Dispenser_A_tr0();
	} 
}

/* The reactions of state Go To Container 4. */
void FSM_HomolPen::react_main_region_Go_To_Container_4()
{
	/* The reactions of state Go To Container 4. */
	if (check_main_region_Go_To_Container_4_tr0_tr0())
	{ 
		effect_main_region_Go_To_Container_4_tr0();
	} 
}

/* The reactions of state Get 4 monocolor cylinders from Dispenser A. */
void FSM_HomolPen::react_main_region_Get_4_monocolor_cylinders_from_Dispenser_A()
{
	/* The reactions of state Get 4 monocolor cylinders from Dispenser A. */
	if (check_main_region_Get_4_monocolor_cylinders_from_Dispenser_A_tr0_tr0())
	{ 
		effect_main_region_Get_4_monocolor_cylinders_from_Dispenser_A_tr0();
	} 
}

/* The reactions of state Poo 4 monocolor cylinders in 4. */
void FSM_HomolPen::react_main_region_Poo_4_monocolor_cylinders_in_4()
{
	/* The reactions of state Poo 4 monocolor cylinders in 4. */
	if (check_main_region_Poo_4_monocolor_cylinders_in_4_tr0_tr0())
	{ 
		effect_main_region_Poo_4_monocolor_cylinders_in_4_tr0();
	} 
}

/* The reactions of state Wait Tration to move. */
void FSM_HomolPen::react_main_region_Wait_Tration_to_move()
{
	/* The reactions of state Wait Tration to move. */
	if (check_main_region_Wait_Tration_to_move_tr0_tr0())
	{ 
		effect_main_region_Wait_Tration_to_move_tr0();
	} 
}

/* The reactions of state Return to Start Area to recal. */
void FSM_HomolPen::react_main_region_Return_to_Start_Area_to_recal()
{
	/* The reactions of state Return to Start Area to recal. */
	if (check_main_region_Return_to_Start_Area_to_recal_tr0_tr0())
	{ 
		effect_main_region_Return_to_Start_Area_to_recal_tr0();
	} 
}

/* The reactions of state Recal X against Start Area flip-flop. */
void FSM_HomolPen::react_main_region_Recal_X_against_Start_Area_flip_flop()
{
	/* The reactions of state Recal X against Start Area flip-flop. */
	if (check_main_region_Recal_X_against_Start_Area_flip_flop_tr0_tr0())
	{ 
		effect_main_region_Recal_X_against_Start_Area_flip_flop_tr0();
	} 
}

/* The reactions of state Recal Y against Start Area border. */
void FSM_HomolPen::react_main_region_Recal_Y_against_Start_Area_border()
{
	/* The reactions of state Recal Y against Start Area border. */
	if (check_main_region_Recal_Y_against_Start_Area_border_tr0_tr0())
	{ 
		effect_main_region_Recal_Y_against_Start_Area_border_tr0();
	} 
}

/* The reactions of state Get C Cylinder. */
void FSM_HomolPen::react_main_region_Get_C_Cylinder()
{
	/* The reactions of state Get C Cylinder. */
	if (check_main_region_Get_C_Cylinder_tr0_tr0())
	{ 
		effect_main_region_Get_C_Cylinder_tr0();
	} 
}

/* The reactions of state Wait for Tration to finish his move. */
void FSM_HomolPen::react_main_region_Wait_for_Tration_to_finish_his_move()
{
	/* The reactions of state Wait for Tration to finish his move. */
	if (check_main_region_Wait_for_Tration_to_finish_his_move_tr0_tr0())
	{ 
		effect_main_region_Wait_for_Tration_to_finish_his_move_tr0();
	} 
}

/* The reactions of state Go To Cylinder C. */
void FSM_HomolPen::react_main_region_Go_To_Cylinder_C()
{
	/* The reactions of state Go To Cylinder C. */
	if (check_main_region_Go_To_Cylinder_C_tr0_tr0())
	{ 
		effect_main_region_Go_To_Cylinder_C_tr0();
	} 
}

/* The reactions of state null. */
void FSM_HomolPen::react_main_region__final_()
{
	/* The reactions of state null. */
}

/* Default react sequence for initial entry  */
void FSM_HomolPen::react_main_region__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Wait_Tration_to_move_default();
}


