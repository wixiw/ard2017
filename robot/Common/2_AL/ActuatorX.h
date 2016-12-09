/* 
* ActuatorX.h
*
* Created: 08/12/2016 22:27:17
* Author: wix
*/


#ifndef __ACTUATORX_H__
#define __ACTUATORX_H__

#include "BSP.h"
#include "RSP.h"

namespace ard
{

	//This is a testing class to represent an actuator system.
	class ActuatorX : public IMiniPeriodicThread
	{
	//functions
	public:
		ActuatorX() = default;
		~ActuatorX() = default;
	
		/**---------------------------------
		 * Container thread interface
		 ---------------------------------*/

		//Implements IMiniThread
		void
		init() override;

		//Implements IMiniThread : method to be called by the container thread
		//                         it's expected to be called periodically
		void
		update (TimeMs sinceLastCall) override;
	
		void
		open();
	
		void
		close();

		void
		arm();

		void
		release();
	
	protected:
	private:
		ActuatorX( const ActuatorX &c );
		ActuatorX& operator=( const ActuatorX &c );
	
		Servo leftClaw;
		Servo rightClaw;
		Servo umbrella;

	}; //ActuatorX

}//namespace ard

#endif //__ACTUATORX_H__
