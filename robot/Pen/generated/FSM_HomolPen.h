
#ifndef FSM_HOMOLPEN_H_
#define FSM_HOMOLPEN_H_

#include "..\..\Common\1_RSP\core\sc_types.h"
#include "StatemachineInterface.h"
#include "..\..\Common\1_RSP\core\TimedStatemachineInterface.h"

/*! \file Header of the state machine 'FSM_HomolPen'.
*/

class FSM_HomolPen : public TimedStatemachineInterface, public StatemachineInterface
{
	
	public:
		
		FSM_HomolPen();
		
		~FSM_HomolPen();
		
		/*! Enumeration of all states */ 
		typedef enum
		{
			main_region_Go_To_Dispenser_A,
			main_region_Go_To_Container_4,
			main_region_Get_4_monocolor_cylinders_from_Dispenser_A,
			main_region_Poo_4_monocolor_cylinders_in_4,
			main_region_Wait_Tration_to_move,
			main_region_Return_to_Start_Area_to_recal,
			main_region_Recal_X_against_Start_Area_flip_flop,
			main_region_Recal_Y_against_Start_Area_border,
			main_region_Get_C_Cylinder,
			main_region_Wait_for_Tration_to_finish_his_move,
			main_region_Go_To_Cylinder_C,
			main_region__final_,
			main_region_Go_to_B,
			main_region_Get_B_Cylinder,
			main_region_Go_to_poo_in_3,
			main_region_Poo_in_3,
			main_region_Avoid_crater_2,
			main_region_Avoid_crater_,
			main_region_Backward_a_bit,
			FSM_HomolPen_last_state
		} FSM_HomolPenStates;
		
		//! Inner class for default interface scope.
		class DefaultSCI
		{
			
			public:
				
			private:
				friend class FSM_HomolPen;
		};
				
				//! Inner class for default interface scope operation callbacks.
				class DefaultSCI_OCB
				{
					public:
						virtual ~DefaultSCI_OCB() = 0;
						
						virtual void startLSA(sc_integer id) = 0;
						
						virtual void stopLSA() = 0;
						
						virtual sc_integer getLSAStatus() = 0;
						
						virtual void goToLSAEntry(sc_integer id, sc_integer dir) = 0;
						
						virtual void beep(sc_integer nb) = 0;
						
						virtual void led1(sc_integer blink) = 0;
						
						virtual void led2(sc_integer blink) = 0;
						
						virtual void led3(sc_integer blink) = 0;
						
						virtual void led4(sc_integer blink) = 0;
						
						virtual void ledRGB(sc_integer color, sc_integer blink) = 0;
						
						virtual sc_boolean isStartPlugged() = 0;
						
						virtual sc_boolean isColorSwitchOnPrefered() = 0;
						
						virtual sc_boolean isUser1SwitchOn() = 0;
						
						virtual sc_boolean isUser2SwitchOn() = 0;
						
						virtual void logDebug(sc_string msg) = 0;
						
						virtual void logInfo(sc_string msg) = 0;
						
						virtual void logError(sc_string msg) = 0;
						
						virtual void dieMotherFucker() = 0;
						
						virtual sc_integer xav() = 0;
						
						virtual sc_integer xar() = 0;
						
						virtual sc_integer yside() = 0;
						
						virtual sc_integer xavExtended() = 0;
						
						virtual sc_integer xouter() = 0;
						
						virtual sc_integer getRemainingTime() = 0;
						
						virtual void setStatus(sc_integer status) = 0;
						
						virtual void enableAvoidance(sc_boolean on) = 0;
						
						virtual void setPosition(sc_real x, sc_real y, sc_real h) = 0;
						
						virtual void setSpeedAcc(sc_integer vMax, sc_integer vMaxTurn, sc_integer accMax, sc_integer accMaxTurn) = 0;
						
						virtual void goTo_ID(sc_real x, sc_real y, sc_integer dir) = 0;
						
						virtual void goToNoSym(sc_real x, sc_real y, sc_integer dir) = 0;
						
						virtual void goToCap(sc_real x, sc_real y, sc_real h, sc_integer dir) = 0;
						
						virtual void goToCapNoSym(sc_real x, sc_real y, sc_real h, sc_integer dir) = 0;
						
						virtual void goForward(sc_real d) = 0;
						
						virtual void turnDelta(sc_real dh) = 0;
						
						virtual void turnTo(sc_real h) = 0;
						
						virtual void faceTo(sc_real x, sc_real y) = 0;
						
						virtual void recalFace(sc_integer border) = 0;
						
						virtual void recalRear(sc_integer border) = 0;
						
						virtual void stopMoving() = 0;
						
						virtual sc_boolean targetReached() = 0;
						
						virtual sc_boolean omronFrontLeft() = 0;
						
						virtual sc_boolean omronFrontRight() = 0;
						
						virtual sc_boolean omronRearLeft() = 0;
						
						virtual sc_boolean omronRearRight() = 0;
						
						virtual sc_boolean switchRecalFL() = 0;
						
						virtual sc_boolean switchRecalFR() = 0;
						
						virtual sc_boolean switchRecalRC() = 0;
						
						virtual void swallow(sc_boolean on) = 0;
						
						virtual void turnWheels(sc_integer on) = 0;
						
						virtual void lifter(sc_boolean up) = 0;
						
						virtual sc_boolean lifterAtTarget() = 0;
						
						virtual void arms(sc_integer left, sc_integer right) = 0;
						
						virtual sc_boolean armsAtTarget() = 0;
						
						virtual void turnCylinder(sc_boolean on) = 0;
						
						virtual void faceUpCylinder() = 0;
						
						virtual sc_integer getFaceUpStatus() = 0;
						
						virtual sc_boolean switchArmLout() = 0;
						
						virtual sc_boolean switchArmLin() = 0;
						
						virtual sc_boolean switchArmRout() = 0;
						
						virtual sc_boolean switchArmRin() = 0;
						
						virtual sc_boolean omronCylinder() = 0;
						
						virtual sc_boolean switchCylinder() = 0;
						
						virtual sc_boolean switchLifterUp() = 0;
						
						virtual sc_boolean switchLifterDown() = 0;
						
						virtual sc_integer stockColor() = 0;
						
						virtual sc_integer score() = 0;
						
						virtual sc_integer robotStockCount() = 0;
						
						virtual sc_integer nextCylinderColor() = 0;
						
						virtual sc_integer containerCount(sc_integer containerId) = 0;
						
						virtual sc_integer dispenserA_Count() = 0;
						
						virtual sc_integer dispenserG_Count() = 0;
						
						virtual sc_integer dispenserOppG_Count() = 0;
						
						virtual void informWithdraw_A(sc_integer nb) = 0;
						
						virtual void informWithdraw_G(sc_integer nb) = 0;
						
						virtual void informWithdraw_OppG(sc_integer nb) = 0;
						
						virtual void informPooed_3(sc_integer nb) = 0;
						
						virtual void informPooed_4(sc_integer nb) = 0;
						
						virtual void informPooed_2(sc_integer nb) = 0;
						
						virtual void informPooed_5(sc_integer nb) = 0;
						
						virtual void informPooed_1(sc_integer nb) = 0;
						
						virtual void informPooed_6(sc_integer nb) = 0;
						
						virtual void informPooed_OnTable(sc_integer nb) = 0;
						
						virtual void informTaken_D() = 0;
						
						virtual void informTaken_F() = 0;
						
						virtual void informTaken_E() = 0;
						
						virtual void informTaken_B() = 0;
						
						virtual void informTaken_C() = 0;
						
						virtual void informTaken_Opp_D() = 0;
						
						virtual void informTaken_Opp_F() = 0;
						
						virtual void informTaken_Opp_E() = 0;
						
						virtual void informPushedAway_D() = 0;
						
						virtual void informPushedAway_F() = 0;
						
						virtual void informPushedAway_E() = 0;
						
						virtual void informPushedAway_B() = 0;
						
						virtual void informPushedAway_C() = 0;
						
						virtual void informPushedAway_Opp_D() = 0;
						
						virtual void informPushedAway_Opp_F() = 0;
						
						virtual void informPushedAway_Opp_E() = 0;
				};
				
				/*! Set the working instance of the operation callback interface 'DefaultSCI_OCB'. */
				void setDefaultSCI_OCB(DefaultSCI_OCB* operationCallback);
		
		/*! Returns an instance of the interface class 'DefaultSCI'. */
		DefaultSCI* getDefaultSCI();
		
		
		
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
		sc_boolean isStateActive(FSM_HomolPenStates state);
	
	private:
	
		//! Inner class for internal interface scope.
		class InternalSCI
		{
			
			public:
				/*! Gets the value of the variable 'FWD' that is defined in the internal scope. */
				sc_integer get_fWD();
				
				/*! Gets the value of the variable 'BWD' that is defined in the internal scope. */
				sc_integer get_bWD();
				
				/*! Gets the value of the variable 'TOP_Y' that is defined in the internal scope. */
				sc_integer get_tOP_Y();
				
				/*! Gets the value of the variable 'OWN_SIDE_X' that is defined in the internal scope. */
				sc_integer get_oWN_SIDE_X();
				
				/*! Gets the value of the variable 'OPP_SIDE_X' that is defined in the internal scope. */
				sc_integer get_oPP_SIDE_X();
				
				/*! Gets the value of the variable 'START_AREA_Y' that is defined in the internal scope. */
				sc_integer get_sTART_AREA_Y();
				
				/*! Gets the value of the variable 'BOT_Y' that is defined in the internal scope. */
				sc_integer get_bOT_Y();
				
				/*! Gets the value of the variable 'START_AREA_X' that is defined in the internal scope. */
				sc_integer get_sTART_AREA_X();
				
				/*! Gets the value of the variable 'LED_OFF' that is defined in the internal scope. */
				sc_integer get_lED_OFF();
				
				/*! Gets the value of the variable 'LED_ON' that is defined in the internal scope. */
				sc_integer get_lED_ON();
				
				/*! Gets the value of the variable 'SLOW_BLINK' that is defined in the internal scope. */
				sc_integer get_sLOW_BLINK();
				
				/*! Gets the value of the variable 'FAST_BLINK' that is defined in the internal scope. */
				sc_integer get_fAST_BLINK();
				
				/*! Gets the value of the variable 'ledRED' that is defined in the internal scope. */
				sc_integer get_ledRED();
				
				/*! Gets the value of the variable 'ledGREEN' that is defined in the internal scope. */
				sc_integer get_ledGREEN();
				
				/*! Gets the value of the variable 'ledBLUE' that is defined in the internal scope. */
				sc_integer get_ledBLUE();
				
				/*! Gets the value of the variable 'ledYELLOW' that is defined in the internal scope. */
				sc_integer get_ledYELLOW();
				
				/*! Gets the value of the variable 'ledCYAN' that is defined in the internal scope. */
				sc_integer get_ledCYAN();
				
				/*! Gets the value of the variable 'ledPURPLE' that is defined in the internal scope. */
				sc_integer get_ledPURPLE();
				
				/*! Gets the value of the variable 'ledWHITE' that is defined in the internal scope. */
				sc_integer get_ledWHITE();
				
				/*! Gets the value of the variable 'cylUNKNOWN' that is defined in the internal scope. */
				sc_integer get_cylUNKNOWN();
				
				/*! Gets the value of the variable 'cylYELLOW' that is defined in the internal scope. */
				sc_integer get_cylYELLOW();
				
				/*! Gets the value of the variable 'cylBLUE' that is defined in the internal scope. */
				sc_integer get_cylBLUE();
				
				/*! Gets the value of the variable 'cylWHITE' that is defined in the internal scope. */
				sc_integer get_cylWHITE();
				
				/*! Gets the value of the variable 'NO_CMD' that is defined in the internal scope. */
				sc_integer get_nO_CMD();
				
				/*! Gets the value of the variable 'FAILED' that is defined in the internal scope. */
				sc_integer get_fAILED();
				
				/*! Gets the value of the variable 'OK' that is defined in the internal scope. */
				sc_integer get_oK();
				
				/*! Gets the value of the variable 'NO_LSA' that is defined in the internal scope. */
				sc_integer get_nO_LSA();
				
				/*! Gets the value of the variable 'SUCCESS' that is defined in the internal scope. */
				sc_integer get_sUCCESS();
				
				/*! Gets the value of the variable 'FAILURE' that is defined in the internal scope. */
				sc_integer get_fAILURE();
				
				/*! Gets the value of the variable 'IN_PROGRESS' that is defined in the internal scope. */
				sc_integer get_iN_PROGRESS();
				
				/*! Gets the value of the variable 'LSA_A' that is defined in the internal scope. */
				sc_integer get_lSA_A();
				
				/*! Gets the value of the variable 'LSA_G' that is defined in the internal scope. */
				sc_integer get_lSA_G();
				
				/*! Gets the value of the variable 'LSA_OppG' that is defined in the internal scope. */
				sc_integer get_lSA_OppG();
				
				/*! Gets the value of the variable 'LSA_1' that is defined in the internal scope. */
				sc_integer get_lSA_1();
				
				/*! Gets the value of the variable 'LSA_2' that is defined in the internal scope. */
				sc_integer get_lSA_2();
				
				/*! Gets the value of the variable 'LSA_3' that is defined in the internal scope. */
				sc_integer get_lSA_3();
				
				/*! Gets the value of the variable 'LSA_4' that is defined in the internal scope. */
				sc_integer get_lSA_4();
				
				/*! Gets the value of the variable 'LSA_5' that is defined in the internal scope. */
				sc_integer get_lSA_5();
				
				/*! Gets the value of the variable 'LSA_B' that is defined in the internal scope. */
				sc_integer get_lSA_B();
				
				/*! Gets the value of the variable 'LSA_C' that is defined in the internal scope. */
				sc_integer get_lSA_C();
				
				/*! Gets the value of the variable 'LSA_D' that is defined in the internal scope. */
				sc_integer get_lSA_D();
				
				/*! Gets the value of the variable 'LSA_E' that is defined in the internal scope. */
				sc_integer get_lSA_E();
				
				/*! Gets the value of the variable 'LSA_F' that is defined in the internal scope. */
				sc_integer get_lSA_F();
				
				/*! Gets the value of the variable 'LSA_OppD' that is defined in the internal scope. */
				sc_integer get_lSA_OppD();
				
				/*! Gets the value of the variable 'LSA_OppE' that is defined in the internal scope. */
				sc_integer get_lSA_OppE();
				
				/*! Gets the value of the variable 'LSA_OppF' that is defined in the internal scope. */
				sc_integer get_lSA_OppF();
				
				/*! Gets the value of the variable 'LSA_6' that is defined in the internal scope. */
				sc_integer get_lSA_6();
				
				/*! Gets the value of the variable 'LSA_IA' that is defined in the internal scope. */
				sc_integer get_lSA_IA();
				
				
			private:
				friend class FSM_HomolPen;
				sc_integer FWD;
				sc_integer BWD;
				sc_integer TOP_Y;
				sc_integer OWN_SIDE_X;
				sc_integer OPP_SIDE_X;
				sc_integer START_AREA_Y;
				sc_integer BOT_Y;
				sc_integer START_AREA_X;
				sc_integer LED_OFF;
				sc_integer LED_ON;
				sc_integer SLOW_BLINK;
				sc_integer FAST_BLINK;
				sc_integer ledRED;
				sc_integer ledGREEN;
				sc_integer ledBLUE;
				sc_integer ledYELLOW;
				sc_integer ledCYAN;
				sc_integer ledPURPLE;
				sc_integer ledWHITE;
				sc_integer cylUNKNOWN;
				sc_integer cylYELLOW;
				sc_integer cylBLUE;
				sc_integer cylWHITE;
				sc_integer NO_CMD;
				sc_integer FAILED;
				sc_integer OK;
				sc_integer NO_LSA;
				sc_integer SUCCESS;
				sc_integer FAILURE;
				sc_integer IN_PROGRESS;
				sc_integer LSA_A;
				sc_integer LSA_G;
				sc_integer LSA_OppG;
				sc_integer LSA_1;
				sc_integer LSA_2;
				sc_integer LSA_3;
				sc_integer LSA_4;
				sc_integer LSA_5;
				sc_integer LSA_B;
				sc_integer LSA_C;
				sc_integer LSA_D;
				sc_integer LSA_E;
				sc_integer LSA_F;
				sc_integer LSA_OppD;
				sc_integer LSA_OppE;
				sc_integer LSA_OppF;
				sc_integer LSA_6;
				sc_integer LSA_IA;
		};
	
		//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
		static const sc_integer maxOrthogonalStates = 1;
		
		//! number of time events used by the state machine.
		static const sc_integer timeEventsCount = 2;
		TimerInterface* timer;
		sc_boolean timeEvents[timeEventsCount];
		
		FSM_HomolPenStates stateConfVector[maxOrthogonalStates];
		
		sc_ushort stateConfVectorPosition;
		
		DefaultSCI iface;
		DefaultSCI_OCB* iface_OCB;
		InternalSCI ifaceInternalSCI;
		
		// prototypes of all internal functions
		
		sc_boolean check_main_region_Go_To_Dispenser_A_tr0_tr0();
		sc_boolean check_main_region_Go_To_Container_4_tr0_tr0();
		sc_boolean check_main_region_Get_4_monocolor_cylinders_from_Dispenser_A_tr0_tr0();
		sc_boolean check_main_region_Poo_4_monocolor_cylinders_in_4_tr0_tr0();
		sc_boolean check_main_region_Wait_Tration_to_move_tr0_tr0();
		sc_boolean check_main_region_Return_to_Start_Area_to_recal_tr0_tr0();
		sc_boolean check_main_region_Recal_X_against_Start_Area_flip_flop_tr0_tr0();
		sc_boolean check_main_region_Recal_Y_against_Start_Area_border_tr0_tr0();
		sc_boolean check_main_region_Get_C_Cylinder_tr0_tr0();
		sc_boolean check_main_region_Wait_for_Tration_to_finish_his_move_tr0_tr0();
		sc_boolean check_main_region_Go_To_Cylinder_C_tr0_tr0();
		sc_boolean check_main_region_Go_to_B_tr0_tr0();
		sc_boolean check_main_region_Get_B_Cylinder_tr0_tr0();
		sc_boolean check_main_region_Go_to_poo_in_3_tr0_tr0();
		sc_boolean check_main_region_Poo_in_3_tr0_tr0();
		sc_boolean check_main_region_Avoid_crater_2_tr0_tr0();
		sc_boolean check_main_region_Avoid_crater__tr0_tr0();
		sc_boolean check_main_region_Backward_a_bit_tr0_tr0();
		void effect_main_region_Go_To_Dispenser_A_tr0();
		void effect_main_region_Go_To_Container_4_tr0();
		void effect_main_region_Get_4_monocolor_cylinders_from_Dispenser_A_tr0();
		void effect_main_region_Poo_4_monocolor_cylinders_in_4_tr0();
		void effect_main_region_Wait_Tration_to_move_tr0();
		void effect_main_region_Return_to_Start_Area_to_recal_tr0();
		void effect_main_region_Recal_X_against_Start_Area_flip_flop_tr0();
		void effect_main_region_Recal_Y_against_Start_Area_border_tr0();
		void effect_main_region_Get_C_Cylinder_tr0();
		void effect_main_region_Wait_for_Tration_to_finish_his_move_tr0();
		void effect_main_region_Go_To_Cylinder_C_tr0();
		void effect_main_region_Go_to_B_tr0();
		void effect_main_region_Get_B_Cylinder_tr0();
		void effect_main_region_Go_to_poo_in_3_tr0();
		void effect_main_region_Poo_in_3_tr0();
		void effect_main_region_Avoid_crater_2_tr0();
		void effect_main_region_Avoid_crater__tr0();
		void effect_main_region_Backward_a_bit_tr0();
		void enact_main_region_Go_To_Dispenser_A();
		void enact_main_region_Go_To_Container_4();
		void enact_main_region_Get_4_monocolor_cylinders_from_Dispenser_A();
		void enact_main_region_Poo_4_monocolor_cylinders_in_4();
		void enact_main_region_Wait_Tration_to_move();
		void enact_main_region_Return_to_Start_Area_to_recal();
		void enact_main_region_Recal_X_against_Start_Area_flip_flop();
		void enact_main_region_Recal_Y_against_Start_Area_border();
		void enact_main_region_Get_C_Cylinder();
		void enact_main_region_Wait_for_Tration_to_finish_his_move();
		void enact_main_region_Go_To_Cylinder_C();
		void enact_main_region_Go_to_B();
		void enact_main_region_Get_B_Cylinder();
		void enact_main_region_Go_to_poo_in_3();
		void enact_main_region_Poo_in_3();
		void enact_main_region_Avoid_crater_2();
		void enact_main_region_Avoid_crater_();
		void enact_main_region_Backward_a_bit();
		void exact_main_region_Get_4_monocolor_cylinders_from_Dispenser_A();
		void exact_main_region_Poo_4_monocolor_cylinders_in_4();
		void exact_main_region_Wait_Tration_to_move();
		void exact_main_region_Get_C_Cylinder();
		void exact_main_region_Wait_for_Tration_to_finish_his_move();
		void exact_main_region_Get_B_Cylinder();
		void exact_main_region_Poo_in_3();
		void enseq_main_region_Go_To_Dispenser_A_default();
		void enseq_main_region_Go_To_Container_4_default();
		void enseq_main_region_Get_4_monocolor_cylinders_from_Dispenser_A_default();
		void enseq_main_region_Poo_4_monocolor_cylinders_in_4_default();
		void enseq_main_region_Wait_Tration_to_move_default();
		void enseq_main_region_Return_to_Start_Area_to_recal_default();
		void enseq_main_region_Recal_X_against_Start_Area_flip_flop_default();
		void enseq_main_region_Recal_Y_against_Start_Area_border_default();
		void enseq_main_region_Get_C_Cylinder_default();
		void enseq_main_region_Wait_for_Tration_to_finish_his_move_default();
		void enseq_main_region_Go_To_Cylinder_C_default();
		void enseq_main_region__final__default();
		void enseq_main_region_Go_to_B_default();
		void enseq_main_region_Get_B_Cylinder_default();
		void enseq_main_region_Go_to_poo_in_3_default();
		void enseq_main_region_Poo_in_3_default();
		void enseq_main_region_Avoid_crater_2_default();
		void enseq_main_region_Avoid_crater__default();
		void enseq_main_region_Backward_a_bit_default();
		void enseq_main_region_default();
		void exseq_main_region_Go_To_Dispenser_A();
		void exseq_main_region_Go_To_Container_4();
		void exseq_main_region_Get_4_monocolor_cylinders_from_Dispenser_A();
		void exseq_main_region_Poo_4_monocolor_cylinders_in_4();
		void exseq_main_region_Wait_Tration_to_move();
		void exseq_main_region_Return_to_Start_Area_to_recal();
		void exseq_main_region_Recal_X_against_Start_Area_flip_flop();
		void exseq_main_region_Recal_Y_against_Start_Area_border();
		void exseq_main_region_Get_C_Cylinder();
		void exseq_main_region_Wait_for_Tration_to_finish_his_move();
		void exseq_main_region_Go_To_Cylinder_C();
		void exseq_main_region__final_();
		void exseq_main_region_Go_to_B();
		void exseq_main_region_Get_B_Cylinder();
		void exseq_main_region_Go_to_poo_in_3();
		void exseq_main_region_Poo_in_3();
		void exseq_main_region_Avoid_crater_2();
		void exseq_main_region_Avoid_crater_();
		void exseq_main_region_Backward_a_bit();
		void exseq_main_region();
		void react_main_region_Go_To_Dispenser_A();
		void react_main_region_Go_To_Container_4();
		void react_main_region_Get_4_monocolor_cylinders_from_Dispenser_A();
		void react_main_region_Poo_4_monocolor_cylinders_in_4();
		void react_main_region_Wait_Tration_to_move();
		void react_main_region_Return_to_Start_Area_to_recal();
		void react_main_region_Recal_X_against_Start_Area_flip_flop();
		void react_main_region_Recal_Y_against_Start_Area_border();
		void react_main_region_Get_C_Cylinder();
		void react_main_region_Wait_for_Tration_to_finish_his_move();
		void react_main_region_Go_To_Cylinder_C();
		void react_main_region__final_();
		void react_main_region_Go_to_B();
		void react_main_region_Get_B_Cylinder();
		void react_main_region_Go_to_poo_in_3();
		void react_main_region_Poo_in_3();
		void react_main_region_Avoid_crater_2();
		void react_main_region_Avoid_crater_();
		void react_main_region_Backward_a_bit();
		void react_main_region__entry_Default();
		void clearInEvents();
		void clearOutEvents();
		
};
inline FSM_HomolPen::DefaultSCI_OCB::~DefaultSCI_OCB() {}
#endif /* FSM_HOMOLPEN_H_ */