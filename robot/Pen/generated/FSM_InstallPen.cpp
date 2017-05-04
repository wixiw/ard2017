
#include "FSM_InstallPen.h"
#include <string.h>

/*! \file Implementation of the state machine 'FSM_InstallPen'
*/

FSM_InstallPen::FSM_InstallPen()
{
	
	iface_OCB = null;
	
	stateConfVectorPosition = 0;
	
	timer = null;
}

FSM_InstallPen::~FSM_InstallPen()
{
}


void FSM_InstallPen::init()
{
	for (int i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = FSM_InstallPen_last_state;
	
	stateConfVectorPosition = 0;

	clearInEvents();
	clearOutEvents();
	
	/* Default init sequence for statechart FSM_InstallPen */
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

void FSM_InstallPen::enter()
{
	/* Default enter sequence for statechart FSM_InstallPen */
	enseq_main_region_default();
}

void FSM_InstallPen::exit()
{
	/* Default exit sequence for statechart FSM_InstallPen */
	exseq_main_region();
}

sc_boolean FSM_InstallPen::isActive()
{
	return stateConfVector[0] != FSM_InstallPen_last_state;
}

sc_boolean FSM_InstallPen::isFinal()
{
	return (stateConfVector[0] == main_region__final_);
}

void FSM_InstallPen::runCycle()
{
	
	clearOutEvents();
	
	for (stateConfVectorPosition = 0;
		stateConfVectorPosition < maxOrthogonalStates;
		stateConfVectorPosition++)
		{
			
		switch (stateConfVector[stateConfVectorPosition])
		{
		case main_region_Recal_on_flip_flop :
		{
			react_main_region_Recal_on_flip_flop();
			break;
		}
		case main_region_Recal_on_start_border :
		{
			react_main_region_Recal_on_start_border();
			break;
		}
		case main_region_Position_robot_for_Y_coordinate :
		{
			react_main_region_Position_robot_for_Y_coordinate();
			break;
		}
		case main_region_Go_in_start_configuration :
		{
			react_main_region_Go_in_start_configuration();
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

void FSM_InstallPen::clearInEvents()
{
	timeEvents[0] = false; 
}

void FSM_InstallPen::clearOutEvents()
{
}


void FSM_InstallPen::setTimer(TimerInterface* timer)
{
	this->timer = timer;
}

TimerInterface* FSM_InstallPen::getTimer()
{
	return timer;
}

void FSM_InstallPen::raiseTimeEvent(sc_eventid evid)
{
	if ((evid >= (sc_eventid)timeEvents) && (evid < (sc_eventid)(&timeEvents[timeEventsCount])))
	{
		*(sc_boolean*)evid = true;
	}				
}

sc_boolean FSM_InstallPen::isStateActive(FSM_InstallPenStates state)
{
	switch (state)
	{
		case main_region_Recal_on_flip_flop : 
			return (sc_boolean) (stateConfVector[0] == main_region_Recal_on_flip_flop
			);
		case main_region_Recal_on_start_border : 
			return (sc_boolean) (stateConfVector[0] == main_region_Recal_on_start_border
			);
		case main_region_Position_robot_for_Y_coordinate : 
			return (sc_boolean) (stateConfVector[0] == main_region_Position_robot_for_Y_coordinate
			);
		case main_region_Go_in_start_configuration : 
			return (sc_boolean) (stateConfVector[0] == main_region_Go_in_start_configuration
			);
		case main_region__final_ : 
			return (sc_boolean) (stateConfVector[0] == main_region__final_
			);
		default: return false;
	}
}

FSM_InstallPen::DefaultSCI* FSM_InstallPen::getDefaultSCI()
{
	return &iface;
}


void FSM_InstallPen::setDefaultSCI_OCB(DefaultSCI_OCB* operationCallback)
{
	iface_OCB = operationCallback;
}

sc_integer FSM_InstallPen::InternalSCI::get_fWD()
{
	return FWD;
}

sc_integer FSM_InstallPen::InternalSCI::get_bWD()
{
	return BWD;
}

sc_integer FSM_InstallPen::InternalSCI::get_tOP_Y()
{
	return TOP_Y;
}

sc_integer FSM_InstallPen::InternalSCI::get_oWN_SIDE_X()
{
	return OWN_SIDE_X;
}

sc_integer FSM_InstallPen::InternalSCI::get_oPP_SIDE_X()
{
	return OPP_SIDE_X;
}

sc_integer FSM_InstallPen::InternalSCI::get_sTART_AREA_Y()
{
	return START_AREA_Y;
}

sc_integer FSM_InstallPen::InternalSCI::get_bOT_Y()
{
	return BOT_Y;
}

sc_integer FSM_InstallPen::InternalSCI::get_sTART_AREA_X()
{
	return START_AREA_X;
}

sc_integer FSM_InstallPen::InternalSCI::get_lED_OFF()
{
	return LED_OFF;
}

sc_integer FSM_InstallPen::InternalSCI::get_lED_ON()
{
	return LED_ON;
}

sc_integer FSM_InstallPen::InternalSCI::get_sLOW_BLINK()
{
	return SLOW_BLINK;
}

sc_integer FSM_InstallPen::InternalSCI::get_fAST_BLINK()
{
	return FAST_BLINK;
}

sc_integer FSM_InstallPen::InternalSCI::get_ledRED()
{
	return ledRED;
}

sc_integer FSM_InstallPen::InternalSCI::get_ledGREEN()
{
	return ledGREEN;
}

sc_integer FSM_InstallPen::InternalSCI::get_ledBLUE()
{
	return ledBLUE;
}

sc_integer FSM_InstallPen::InternalSCI::get_ledYELLOW()
{
	return ledYELLOW;
}

sc_integer FSM_InstallPen::InternalSCI::get_ledCYAN()
{
	return ledCYAN;
}

sc_integer FSM_InstallPen::InternalSCI::get_ledPURPLE()
{
	return ledPURPLE;
}

sc_integer FSM_InstallPen::InternalSCI::get_ledWHITE()
{
	return ledWHITE;
}

sc_integer FSM_InstallPen::InternalSCI::get_cylUNKNOWN()
{
	return cylUNKNOWN;
}

sc_integer FSM_InstallPen::InternalSCI::get_cylYELLOW()
{
	return cylYELLOW;
}

sc_integer FSM_InstallPen::InternalSCI::get_cylBLUE()
{
	return cylBLUE;
}

sc_integer FSM_InstallPen::InternalSCI::get_cylWHITE()
{
	return cylWHITE;
}

sc_integer FSM_InstallPen::InternalSCI::get_nO_CMD()
{
	return NO_CMD;
}

sc_integer FSM_InstallPen::InternalSCI::get_fAILED()
{
	return FAILED;
}

sc_integer FSM_InstallPen::InternalSCI::get_oK()
{
	return OK;
}

sc_integer FSM_InstallPen::InternalSCI::get_nO_LSA()
{
	return NO_LSA;
}

sc_integer FSM_InstallPen::InternalSCI::get_sUCCESS()
{
	return SUCCESS;
}

sc_integer FSM_InstallPen::InternalSCI::get_fAILURE()
{
	return FAILURE;
}

sc_integer FSM_InstallPen::InternalSCI::get_iN_PROGRESS()
{
	return IN_PROGRESS;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_A()
{
	return LSA_A;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_G()
{
	return LSA_G;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_OppG()
{
	return LSA_OppG;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_1()
{
	return LSA_1;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_2()
{
	return LSA_2;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_3()
{
	return LSA_3;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_4()
{
	return LSA_4;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_5()
{
	return LSA_5;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_B()
{
	return LSA_B;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_C()
{
	return LSA_C;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_D()
{
	return LSA_D;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_E()
{
	return LSA_E;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_F()
{
	return LSA_F;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_OppD()
{
	return LSA_OppD;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_OppE()
{
	return LSA_OppE;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_OppF()
{
	return LSA_OppF;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_6()
{
	return LSA_6;
}

sc_integer FSM_InstallPen::InternalSCI::get_lSA_IA()
{
	return LSA_IA;
}


// implementations of all internal functions

sc_boolean FSM_InstallPen::check_main_region_Recal_on_flip_flop_tr0_tr0()
{
	return (timeEvents[0]) && (false);
}

sc_boolean FSM_InstallPen::check_main_region_Recal_on_flip_flop_tr1_tr1()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_InstallPen::check_main_region_Recal_on_start_border_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_InstallPen::check_main_region_Position_robot_for_Y_coordinate_tr0_tr0()
{
	return iface_OCB->targetReached();
}

sc_boolean FSM_InstallPen::check_main_region_Go_in_start_configuration_tr0_tr0()
{
	return iface_OCB->targetReached();
}

void FSM_InstallPen::effect_main_region_Recal_on_flip_flop_tr0()
{
	exseq_main_region_Recal_on_flip_flop();
	enseq_main_region_Recal_on_flip_flop_default();
}

void FSM_InstallPen::effect_main_region_Recal_on_flip_flop_tr1()
{
	exseq_main_region_Recal_on_flip_flop();
	enseq_main_region_Recal_on_start_border_default();
}

void FSM_InstallPen::effect_main_region_Recal_on_start_border_tr0()
{
	exseq_main_region_Recal_on_start_border();
	enseq_main_region_Position_robot_for_Y_coordinate_default();
}

void FSM_InstallPen::effect_main_region_Position_robot_for_Y_coordinate_tr0()
{
	exseq_main_region_Position_robot_for_Y_coordinate();
	enseq_main_region_Go_in_start_configuration_default();
}

void FSM_InstallPen::effect_main_region_Go_in_start_configuration_tr0()
{
	exseq_main_region_Go_in_start_configuration();
	enseq_main_region__final__default();
}

/* Entry action for state 'Recal on flip-flop'. */
void FSM_InstallPen::enact_main_region_Recal_on_flip_flop()
{
	/* Entry action for state 'Recal on flip-flop'. */
	timer->setTimer(this, (sc_eventid)(&timeEvents[0]), 1 * 1000, false);
	iface_OCB->setPosition(555, 730, 180);
	iface_OCB->recalRear(ifaceInternalSCI.START_AREA_X);
}

/* Entry action for state 'Recal on start border'. */
void FSM_InstallPen::enact_main_region_Recal_on_start_border()
{
	/* Entry action for state 'Recal on start border'. */
	iface_OCB->recalFace(ifaceInternalSCI.TOP_Y);
}

/* Entry action for state 'Position robot for Y coordinate'. */
void FSM_InstallPen::enact_main_region_Position_robot_for_Y_coordinate()
{
	/* Entry action for state 'Position robot for Y coordinate'. */
	iface_OCB->goTo_ID(590, 730, ifaceInternalSCI.BWD);
}

/* Entry action for state 'Go in start configuration'. */
void FSM_InstallPen::enact_main_region_Go_in_start_configuration()
{
	/* Entry action for state 'Go in start configuration'. */
	iface_OCB->goTo_ID(640, 730, ifaceInternalSCI.FWD);
}

/* Exit action for state 'Recal on flip-flop'. */
void FSM_InstallPen::exact_main_region_Recal_on_flip_flop()
{
	/* Exit action for state 'Recal on flip-flop'. */
	timer->unsetTimer(this, (sc_eventid)(&timeEvents[0]));
}

/* 'default' enter sequence for state Recal on flip-flop */
void FSM_InstallPen::enseq_main_region_Recal_on_flip_flop_default()
{
	/* 'default' enter sequence for state Recal on flip-flop */
	enact_main_region_Recal_on_flip_flop();
	stateConfVector[0] = main_region_Recal_on_flip_flop;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Recal on start border */
void FSM_InstallPen::enseq_main_region_Recal_on_start_border_default()
{
	/* 'default' enter sequence for state Recal on start border */
	enact_main_region_Recal_on_start_border();
	stateConfVector[0] = main_region_Recal_on_start_border;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Position robot for Y coordinate */
void FSM_InstallPen::enseq_main_region_Position_robot_for_Y_coordinate_default()
{
	/* 'default' enter sequence for state Position robot for Y coordinate */
	enact_main_region_Position_robot_for_Y_coordinate();
	stateConfVector[0] = main_region_Position_robot_for_Y_coordinate;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Go in start configuration */
void FSM_InstallPen::enseq_main_region_Go_in_start_configuration_default()
{
	/* 'default' enter sequence for state Go in start configuration */
	enact_main_region_Go_in_start_configuration();
	stateConfVector[0] = main_region_Go_in_start_configuration;
	stateConfVectorPosition = 0;
}

/* Default enter sequence for state null */
void FSM_InstallPen::enseq_main_region__final__default()
{
	/* Default enter sequence for state null */
	stateConfVector[0] = main_region__final_;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
void FSM_InstallPen::enseq_main_region_default()
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default();
}

/* Default exit sequence for state Recal on flip-flop */
void FSM_InstallPen::exseq_main_region_Recal_on_flip_flop()
{
	/* Default exit sequence for state Recal on flip-flop */
	stateConfVector[0] = FSM_InstallPen_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_Recal_on_flip_flop();
}

/* Default exit sequence for state Recal on start border */
void FSM_InstallPen::exseq_main_region_Recal_on_start_border()
{
	/* Default exit sequence for state Recal on start border */
	stateConfVector[0] = FSM_InstallPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Position robot for Y coordinate */
void FSM_InstallPen::exseq_main_region_Position_robot_for_Y_coordinate()
{
	/* Default exit sequence for state Position robot for Y coordinate */
	stateConfVector[0] = FSM_InstallPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for state Go in start configuration */
void FSM_InstallPen::exseq_main_region_Go_in_start_configuration()
{
	/* Default exit sequence for state Go in start configuration */
	stateConfVector[0] = FSM_InstallPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for final state. */
void FSM_InstallPen::exseq_main_region__final_()
{
	/* Default exit sequence for final state. */
	stateConfVector[0] = FSM_InstallPen_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
void FSM_InstallPen::exseq_main_region()
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of FSM_InstallPen.main_region) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case main_region_Recal_on_flip_flop :
		{
			exseq_main_region_Recal_on_flip_flop();
			break;
		}
		case main_region_Recal_on_start_border :
		{
			exseq_main_region_Recal_on_start_border();
			break;
		}
		case main_region_Position_robot_for_Y_coordinate :
		{
			exseq_main_region_Position_robot_for_Y_coordinate();
			break;
		}
		case main_region_Go_in_start_configuration :
		{
			exseq_main_region_Go_in_start_configuration();
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

/* The reactions of state Recal on flip-flop. */
void FSM_InstallPen::react_main_region_Recal_on_flip_flop()
{
	/* The reactions of state Recal on flip-flop. */
	if (check_main_region_Recal_on_flip_flop_tr0_tr0())
	{ 
		effect_main_region_Recal_on_flip_flop_tr0();
	}  else
	{
		if (check_main_region_Recal_on_flip_flop_tr1_tr1())
		{ 
			effect_main_region_Recal_on_flip_flop_tr1();
		} 
	}
}

/* The reactions of state Recal on start border. */
void FSM_InstallPen::react_main_region_Recal_on_start_border()
{
	/* The reactions of state Recal on start border. */
	if (check_main_region_Recal_on_start_border_tr0_tr0())
	{ 
		effect_main_region_Recal_on_start_border_tr0();
	} 
}

/* The reactions of state Position robot for Y coordinate. */
void FSM_InstallPen::react_main_region_Position_robot_for_Y_coordinate()
{
	/* The reactions of state Position robot for Y coordinate. */
	if (check_main_region_Position_robot_for_Y_coordinate_tr0_tr0())
	{ 
		effect_main_region_Position_robot_for_Y_coordinate_tr0();
	} 
}

/* The reactions of state Go in start configuration. */
void FSM_InstallPen::react_main_region_Go_in_start_configuration()
{
	/* The reactions of state Go in start configuration. */
	if (check_main_region_Go_in_start_configuration_tr0_tr0())
	{ 
		effect_main_region_Go_in_start_configuration_tr0();
	} 
}

/* The reactions of state null. */
void FSM_InstallPen::react_main_region__final_()
{
	/* The reactions of state null. */
}

/* Default react sequence for initial entry  */
void FSM_InstallPen::react_main_region__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Recal_on_flip_flop_default();
}


