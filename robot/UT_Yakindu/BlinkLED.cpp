
#include "BlinkLED.h"
#include <string.h>

/*! \file Implementation of the state machine 'blinkLED'
*/

BlinkLED::BlinkLED()
{
	
	ifaceBSP_OCB = null;
	
	stateConfVectorPosition = 0;
	
	timer = null;
}

BlinkLED::~BlinkLED()
{
}


void BlinkLED::init()
{
	for (int i = 0; i < maxOrthogonalStates; ++i)
		stateConfVector[i] = BlinkLED_last_state;
	
	stateConfVectorPosition = 0;

	clearInEvents();
	clearOutEvents();
	
}

void BlinkLED::enter()
{
	/* Default enter sequence for statechart blinkLED */
	enseq_main_region_default();
}

void BlinkLED::exit()
{
	/* Default exit sequence for statechart blinkLED */
	exseq_main_region();
}

sc_boolean BlinkLED::isActive()
{
	return stateConfVector[0] != BlinkLED_last_state;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean BlinkLED::isFinal()
{
   return false;}

void BlinkLED::runCycle()
{
	
	clearOutEvents();
	
	for (stateConfVectorPosition = 0;
		stateConfVectorPosition < maxOrthogonalStates;
		stateConfVectorPosition++)
		{
			
		switch (stateConfVector[stateConfVectorPosition])
		{
		case main_region_LEDOff :
		{
			react_main_region_LEDOff();
			break;
		}
		case main_region_LEDOn :
		{
			react_main_region_LEDOn();
			break;
		}
		case main_region_Init :
		{
			react_main_region_Init();
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents();
}

void BlinkLED::clearInEvents()
{
	iface.start_raised = false;
	timeEvents[0] = false; 
	timeEvents[1] = false; 
}

void BlinkLED::clearOutEvents()
{
}


void BlinkLED::setTimer(TimerInterface* timer)
{
	this->timer = timer;
}

TimerInterface* BlinkLED::getTimer()
{
	return timer;
}

void BlinkLED::raiseTimeEvent(sc_eventid evid)
{
	if ((evid >= (sc_eventid)timeEvents) && (evid < (sc_eventid)(&timeEvents[timeEventsCount])))
	{
		*(sc_boolean*)evid = true;
	}				
}

sc_boolean BlinkLED::isStateActive(BlinkLEDStates state)
{
	switch (state)
	{
		case main_region_LEDOff : 
			return (sc_boolean) (stateConfVector[0] == main_region_LEDOff
			);
		case main_region_LEDOn : 
			return (sc_boolean) (stateConfVector[0] == main_region_LEDOn
			);
		case main_region_Init : 
			return (sc_boolean) (stateConfVector[0] == main_region_Init
			);
		default: return false;
	}
}

BlinkLED::DefaultSCI* BlinkLED::getDefaultSCI()
{
	return &iface;
}

void BlinkLED::DefaultSCI::raise_start()
{
	start_raised = true;
}

void BlinkLED::raise_start()
{
	iface.raise_start();
}


BlinkLED::SCI_BSP* BlinkLED::getSCI_BSP()
{
	return &ifaceBSP;
}


void BlinkLED::setSCI_BSP_OCB(SCI_BSP_OCB* operationCallback)
{
	ifaceBSP_OCB = operationCallback;
}

// implementations of all internal functions

sc_boolean BlinkLED::check_main_region_LEDOff_tr0_tr0()
{
	return timeEvents[0];
}

sc_boolean BlinkLED::check_main_region_LEDOn_tr0_tr0()
{
	return timeEvents[1];
}

sc_boolean BlinkLED::check_main_region_Init_tr0_tr0()
{
	return iface.start_raised;
}

void BlinkLED::effect_main_region_LEDOff_tr0()
{
	exseq_main_region_LEDOff();
	enseq_main_region_LEDOn_default();
}

void BlinkLED::effect_main_region_LEDOn_tr0()
{
	exseq_main_region_LEDOn();
	enseq_main_region_LEDOff_default();
}

void BlinkLED::effect_main_region_Init_tr0()
{
	exseq_main_region_Init();
	enseq_main_region_LEDOff_default();
}

/* Entry action for state 'LEDOff'. */
void BlinkLED::enact_main_region_LEDOff()
{
	/* Entry action for state 'LEDOff'. */
	timer->setTimer(this, (sc_eventid)(&timeEvents[0]), 1 * 1000, false);
	ifaceBSP_OCB->setLed(0);
}

/* Entry action for state 'LEDOn'. */
void BlinkLED::enact_main_region_LEDOn()
{
	/* Entry action for state 'LEDOn'. */
	timer->setTimer(this, (sc_eventid)(&timeEvents[1]), 1 * 1000, false);
	ifaceBSP_OCB->setLed(1);
}

/* Exit action for state 'LEDOff'. */
void BlinkLED::exact_main_region_LEDOff()
{
	/* Exit action for state 'LEDOff'. */
	timer->unsetTimer(this, (sc_eventid)(&timeEvents[0]));
}

/* Exit action for state 'LEDOn'. */
void BlinkLED::exact_main_region_LEDOn()
{
	/* Exit action for state 'LEDOn'. */
	timer->unsetTimer(this, (sc_eventid)(&timeEvents[1]));
}

/* 'default' enter sequence for state LEDOff */
void BlinkLED::enseq_main_region_LEDOff_default()
{
	/* 'default' enter sequence for state LEDOff */
	enact_main_region_LEDOff();
	stateConfVector[0] = main_region_LEDOff;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state LEDOn */
void BlinkLED::enseq_main_region_LEDOn_default()
{
	/* 'default' enter sequence for state LEDOn */
	enact_main_region_LEDOn();
	stateConfVector[0] = main_region_LEDOn;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state Init */
void BlinkLED::enseq_main_region_Init_default()
{
	/* 'default' enter sequence for state Init */
	stateConfVector[0] = main_region_Init;
	stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
void BlinkLED::enseq_main_region_default()
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default();
}

/* Default exit sequence for state LEDOff */
void BlinkLED::exseq_main_region_LEDOff()
{
	/* Default exit sequence for state LEDOff */
	stateConfVector[0] = BlinkLED_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_LEDOff();
}

/* Default exit sequence for state LEDOn */
void BlinkLED::exseq_main_region_LEDOn()
{
	/* Default exit sequence for state LEDOn */
	stateConfVector[0] = BlinkLED_last_state;
	stateConfVectorPosition = 0;
	exact_main_region_LEDOn();
}

/* Default exit sequence for state Init */
void BlinkLED::exseq_main_region_Init()
{
	/* Default exit sequence for state Init */
	stateConfVector[0] = BlinkLED_last_state;
	stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
void BlinkLED::exseq_main_region()
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of blinkLED.main_region) at position 0... */
	switch(stateConfVector[ 0 ])
	{
		case main_region_LEDOff :
		{
			exseq_main_region_LEDOff();
			break;
		}
		case main_region_LEDOn :
		{
			exseq_main_region_LEDOn();
			break;
		}
		case main_region_Init :
		{
			exseq_main_region_Init();
			break;
		}
		default: break;
	}
}

/* The reactions of state LEDOff. */
void BlinkLED::react_main_region_LEDOff()
{
	/* The reactions of state LEDOff. */
	if (check_main_region_LEDOff_tr0_tr0())
	{ 
		effect_main_region_LEDOff_tr0();
	} 
}

/* The reactions of state LEDOn. */
void BlinkLED::react_main_region_LEDOn()
{
	/* The reactions of state LEDOn. */
	if (check_main_region_LEDOn_tr0_tr0())
	{ 
		effect_main_region_LEDOn_tr0();
	} 
}

/* The reactions of state Init. */
void BlinkLED::react_main_region_Init()
{
	/* The reactions of state Init. */
	if (check_main_region_Init_tr0_tr0())
	{ 
		effect_main_region_Init_tr0();
	} 
}

/* Default react sequence for initial entry  */
void BlinkLED::react_main_region__entry_Default()
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Init_default();
}


