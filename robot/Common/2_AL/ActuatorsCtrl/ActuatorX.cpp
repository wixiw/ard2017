/* 
 * ActuatorX.cpp
 *
 * Created: 08/12/2016 22:27:16
 * Author: wix
 */

#include "ActuatorX.h"

#ifdef BUILD_STRATEGY

using namespace ard;

#define CLAW_LEFT_CLOSED 14
#define CLAW_LEFT_OPENED 160
#define CLAW_RIGHT_CLOSED 170
#define CLAW_RIGHT_OPENED 15
#define UMBRELLA_ARMED 30
#define UMBRELLA_RELEASED 150

void
ActuatorX::init ()
{
  leftClaw.attach (SERVO1);
  rightClaw.attach (SERVO5);
  umbrella.attach(SERVO8);
  
  close ();
  umbrella.write (UMBRELLA_RELEASED);
}

void
ActuatorX::update (TimeMs sinceLastCall)
{

}

void
ActuatorX::open ()
{
  leftClaw.write (CLAW_LEFT_OPENED);
  rightClaw.write (CLAW_RIGHT_OPENED);
}

void
ActuatorX::close ()
{
  leftClaw.write (CLAW_LEFT_CLOSED);
  rightClaw.write (CLAW_RIGHT_CLOSED);
}

void
ActuatorX::arm ()
{
  umbrella.write (UMBRELLA_ARMED);
}

void
ActuatorX::release ()
{
  umbrella.write (UMBRELLA_RELEASED);
}

#endif
