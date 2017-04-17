
#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "sc_types.h"
#include "StatemachineInterface.h"
#include "TimedStatemachineInterface.h"

/*! \file Header of the state machine 'blinkLED'.
*/

class BlinkLED : public TimedStatemachineInterface, public StatemachineInterface
{
	
	public:
		
		BlinkLED();
		
		~BlinkLED();
		
		/*! Enumeration of all states */ 
		typedef enum
		{
			main_region_LEDOff,
			main_region_LEDOn,
			main_region_Init,
			BlinkLED_last_state
		} BlinkLEDStates;
		
		//! Inner class for default interface scope.
		class DefaultSCI
		{
			
			public:
				/*! Raises the in event 'start' that is defined in the default interface scope. */
				void raise_start();
				
				
			private:
				friend class BlinkLED;
				sc_boolean start_raised;
		};
				
		
		/*! Returns an instance of the interface class 'DefaultSCI'. */
		DefaultSCI* getDefaultSCI();
		
		/*! Raises the in event 'start' that is defined in the default interface scope. */
		void raise_start();
		
		//! Inner class for BSP interface scope.
		class SCI_BSP
		{
			
			public:
				
			private:
				friend class BlinkLED;
		};
				
				//! Inner class for BSP interface scope operation callbacks.
				class SCI_BSP_OCB
				{
					public:
						virtual ~SCI_BSP_OCB() = 0;
						
						virtual void setLed(sc_integer on) = 0;
				};
				
				/*! Set the working instance of the operation callback interface 'SCI_BSP_OCB'. */
				void setSCI_BSP_OCB(SCI_BSP_OCB* operationCallback);
		
		/*! Returns an instance of the interface class 'SCI_BSP'. */
		SCI_BSP* getSCI_BSP();
		
		
		void init();
		
		void enter();
		
		void exit();
		
		void runCycle();
		
		/*!
		* Checks if the state machine is active (until 2.4.1 this method was used for states).
		* A state machine is active if it has been entered. It is inactive if it has not been entered at all or if it has been exited.
		*/
		sc_boolean isActive();
		
		
		/*!
		* Checks if all active states are final. 
		* If there are no active states then the state machine is considered being inactive. In this case this method returns false.
		*/
		sc_boolean isFinal();
		
		void setTimer(TimerInterface* timer);
		
		TimerInterface* getTimer();
		
		void raiseTimeEvent(sc_eventid event);
		
		/*! Checks if the specified state is active (until 2.4.1 the used method for states was calles isActive()). */
		sc_boolean isStateActive(BlinkLEDStates state);
	
	private:
	
	
		//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
		static const sc_integer maxOrthogonalStates = 1;
		
		//! number of time events used by the state machine.
		static const sc_integer timeEventsCount = 2;
		TimerInterface* timer;
		sc_boolean timeEvents[timeEventsCount];
		
		BlinkLEDStates stateConfVector[maxOrthogonalStates];
		
		sc_ushort stateConfVectorPosition;
		
		DefaultSCI iface;
		SCI_BSP ifaceBSP;
		SCI_BSP_OCB* ifaceBSP_OCB;
		
		// prototypes of all internal functions
		
		sc_boolean check_main_region_LEDOff_tr0_tr0();
		sc_boolean check_main_region_LEDOn_tr0_tr0();
		sc_boolean check_main_region_Init_tr0_tr0();
		void effect_main_region_LEDOff_tr0();
		void effect_main_region_LEDOn_tr0();
		void effect_main_region_Init_tr0();
		void enact_main_region_LEDOff();
		void enact_main_region_LEDOn();
		void exact_main_region_LEDOff();
		void exact_main_region_LEDOn();
		void enseq_main_region_LEDOff_default();
		void enseq_main_region_LEDOn_default();
		void enseq_main_region_Init_default();
		void enseq_main_region_default();
		void exseq_main_region_LEDOff();
		void exseq_main_region_LEDOn();
		void exseq_main_region_Init();
		void exseq_main_region();
		void react_main_region_LEDOff();
		void react_main_region_LEDOn();
		void react_main_region_Init();
		void react_main_region__entry_Default();
		void clearInEvents();
		void clearOutEvents();
		
};
inline BlinkLED::SCI_BSP_OCB::~SCI_BSP_OCB() {}
#endif /* BLINKLED_H_ */
