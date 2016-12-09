#include <Arduino.h>
#include "LogThread.h"
#include "Navigation.h"
#include "K_constants.h"
#include "BSP.h"

using namespace ard;

Navigation::Navigation () :
    m_pose (), m_state (eNavState::IDLE), m_target (), m_sensTarget (
	SENS_UNDEFINED), m_order (eNavOrder::NOTHING), m_angleToTarget (0), m_distanceToTarget (
	0), stepperG (AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR), stepperD (
	AccelStepper::DRIVER, PAPD_STEP, PAPD_DIR), omronFrontLeft (OMRON1, 50,
								    50), omronFrontRight (
	OMRON2, 50, 50), omronRearLeft (OMRON3, 50, 50), omronRearRight (OMRON4,
									 50,
									 50), m_color (
	eColor::UNDEF), m_speed (SPEED_MAX), m_speed_virage (SPEED_MAX_VIR), m_mutex (
    NULL), m_targetReached (NULL)
{
}


/**---------------------------------
 * Container thread interface
 ---------------------------------*/

void
Navigation::init ()
{
  m_mutex = g_ArdOs.Mutex_create();
  m_targetReached = g_ArdOs.Signal_create();
  stepperG.setAcceleration(ACC_MAX);
  stepperD.setAcceleration(ACC_MAX);
}

void
Navigation::update (TimeMs sinceLastCall)
{
  static eNavState lastState = m_state;
  if( m_state != lastState )
    {
        LOG(DEBUG, "NAV : state changed from  " + stateToString (lastState) + " to " + stateToString (m_state));
        lastState = m_state;
    }

  //Take a mutex to prevent localisation and target to be changed during a cycle
  g_ArdOs.Mutex_lock(m_mutex);

  switch (m_state)
    {
    default:
    case eNavState::IDLE:
      {
	switch (m_order)
	  {
	  default:
	  case eNavOrder::NOTHING:
	    {
	      break;
	    }

	  case eNavOrder::GOTO:
	  case eNavOrder::GOTO_CAP:
	    {
	      LOG(INFO,
		  "NAV : new order " + orderToString (m_order) + "(" + m_target.x
		  + ", " + m_target.y + ", " + m_target.h + ") "
		  + sensToString (m_sensTarget) + ".");
	      if (m_sensTarget == SENS_AR)
	      m_angleToTarget = moduloPiPi (m_pose.angleTo (m_target) + M_PI);
	      else
	      m_angleToTarget = m_pose.angleTo (m_target);
	      turn (m_angleToTarget);
	      m_state = eNavState::FACING_DEST;
	      break;
	    }
	}
      break;
    }

  case eNavState::FACING_DEST:
    {
      if (subOrderFinished ())
	{
	  // Update odom
	  m_pose.h += m_angleToTarget;
	  m_pose.h = moduloPiPi (m_pose.h);

	  //Request straight line
	  m_distanceToTarget = m_sensTarget * m_pose.distanceTo (m_target);
	  straight (m_distanceToTarget);

	  //Change state
	  m_angleToTarget = 0;
	  m_state = eNavState::GOING_TO_TARGET;
	  LOG(DEBUG, "NAV : facing destination, beginning line.");
	}
      break;
    }

  case eNavState::GOING_TO_TARGET:
    {
      if (subOrderFinished ())
	{
	  //Update odom
	  m_pose.x += m_distanceToTarget * cos (m_pose.h);
	  m_pose.y += m_distanceToTarget * sin (m_pose.h);

	  //Request rotation to final heading
	  if (m_order == eNavOrder::GOTO_CAP)
	    {
	      if (m_target.h - m_pose.h < -M_PI)
	      m_angleToTarget = m_target.h - m_pose.h + 2 * M_PI;
	      else if (m_target.h - m_pose.h > M_PI)
	      m_angleToTarget = m_target.h - m_pose.h - 2 * M_PI;
	      else
	      m_angleToTarget = m_target.h - m_pose.h;
	      turn (m_angleToTarget);
	    }

	  //Change state
	  m_distanceToTarget = 0;
	  m_state = eNavState::TURNING_AT_TARGET;
	}
      break;
    }

  case eNavState::TURNING_AT_TARGET:
    {
      if (subOrderFinished ())
	{
	  m_angleToTarget = 0;
	  m_state = eNavState::IDLE;
	  m_order = eNavOrder::NOTHING;
	  g_ArdOs.Signal_set(m_targetReached);
	  LOG(INFO, "NAV : order finished.");
	}
      break;
    }

  //TODO attention si on s'arrete avec ca, on ne compte pas le deplacement car c'est normalement fait a la fin
  case eNavState::STOPPING:
    {
      if (subOrderFinished ())
	    {
    	    m_state = eNavState::IDLE;
    	    LOG(INFO, "NAV : stopped.");
	    }
      break;
    }
}

  g_ArdOs.Mutex_unlock(m_mutex);
}

void
Navigation::updateFromInterrupt ()
{
  stepperG.run ();
  stepperD.run ();
  //TODO compute_odom ?
}

/**---------------------------------
 * User (= strategy) interface
 ---------------------------------*/

void
Navigation::setPosition (PointCap newPose)
{
  m_pose = newPose.toAmbiPose (m_color);
  LOG(INFO, "NAV : position set to :" + newPose.toString ());
}

void
Navigation::goTo (Point target, sens_t sens)
{
  g_ArdOs.Mutex_lock(m_mutex);
  //If an order is present, wait
  if (m_state != eNavState::IDLE)
    {
      LOG(INFO, "NAV : new order pending until current order is finished");
      g_ArdOs.Mutex_unlock(m_mutex);
      wait ();
      g_ArdOs.Mutex_lock(m_mutex);
    }

  m_order = eNavOrder::GOTO;
  m_target = target.toAmbiPose (m_color);
  m_sensTarget = sens;

  g_ArdOs.Mutex_unlock(m_mutex);
}

void
Navigation::goToCap (PointCap target, sens_t sens)
{
  g_ArdOs.Mutex_lock(m_mutex);

  //If an order is present, wait
  if (m_state != eNavState::IDLE)
    {
      LOG(INFO, "NAV : new order pending until current order is finished");
      g_ArdOs.Mutex_unlock(m_mutex);
      wait ();
      g_ArdOs.Mutex_lock(m_mutex);
    }

  m_order = eNavOrder::GOTO_CAP;
  m_target = target.toAmbiPose (m_color);
  m_sensTarget = sens;

  g_ArdOs.Mutex_unlock(m_mutex);
}

void
Navigation::goForward (float distanceMm)
{
  ardAssert(false, "not implemented");
  g_ArdOs.Mutex_lock(m_mutex);

  //If an order is present, wait
  if (m_state != eNavState::IDLE)
    {
      LOG(INFO, "NAV : new order pending until current order is finished");
      g_ArdOs.Mutex_unlock(m_mutex);
      wait ();
      g_ArdOs.Mutex_lock(m_mutex);
    }

  //TODO
//  m_order = XXX;
//  m_target = target.toAmbiPose(m_color);
//  m_sensTarget = sens;

  g_ArdOs.Mutex_unlock(m_mutex);
}

void
Navigation::turnTo (float angle)
{
  ardAssert(false, "not implemented");
  g_ArdOs.Mutex_lock(m_mutex);

  //If an order is present, wait
  if (m_state != eNavState::IDLE)
    {
      LOG(INFO, "NAV : new order pending until current order is finished");
      g_ArdOs.Mutex_unlock(m_mutex);
      wait ();
      g_ArdOs.Mutex_lock(m_mutex);
    }

  //TODO
//  m_order = XXX;
//  m_target = target.toAmbiPose(m_color);
//  m_sensTarget = sens;

  g_ArdOs.Mutex_unlock(m_mutex);
}

void
Navigation::faceTo (Point p)
{
  ardAssert(false, "not implemented");
  g_ArdOs.Mutex_lock(m_mutex);

  //If an order is present, wait
  if (m_state != eNavState::IDLE)
    {
      LOG(INFO, "NAV : new order pending until current order is finished");
      g_ArdOs.Mutex_unlock(m_mutex);
      wait ();
      g_ArdOs.Mutex_lock(m_mutex);
    }

  //TODO
//  m_order = XXX;
//  m_target = target.toAmbiPose(m_color);
//  m_sensTarget = sens;

  g_ArdOs.Mutex_unlock(m_mutex);
}

void
Navigation::stop ()
{
    LOG(INFO, "NAV : stop requested");
    g_ArdOs.Mutex_lock(m_mutex);

    stepperG.stop();
    stepperD.stop();

    //The state is directly changed to interrupting
    m_state = eNavState::STOPPING;
    m_order = Navigation::eNavOrder::NOTHING;

    g_ArdOs.Mutex_unlock(m_mutex);
}

void
Navigation::wait ()
{
  //obvisouly don't put a mutex, it's a blocking call ... !
  g_ArdOs.Signal_wait(m_targetReached);
}

bool
Navigation::targetReached ()
{
  g_ArdOs.Mutex_lock(m_mutex);
  bool res = m_state == eNavState::IDLE;
  g_ArdOs.Mutex_unlock(m_mutex);
  return res;
}

/**---------------------------------
 * Nav configuration
 ---------------------------------*/

void
Navigation::setColor (eColor c)
{
  ardAssert(c != eColor::UNDEF, "NAV : color should not be set to undefined.");
  g_ArdOs.Mutex_lock(m_mutex);
  m_color = c;
  g_ArdOs.Mutex_unlock(m_mutex);
}

void
Navigation::setSpeed (float s)
{
  g_ArdOs.Mutex_lock(m_mutex);
  if (s > 0)
    {
      stepperG.setMaxSpeed (s);
      stepperD.setMaxSpeed (s);
      m_speed = s;
    }
  else
    {
      stepperG.setMaxSpeed (SPEED_MAX);
      stepperD.setMaxSpeed (SPEED_MAX);
      m_speed = SPEED_MAX;
    }
  g_ArdOs.Mutex_unlock(m_mutex);
}

void
Navigation::setSpeedVir (float s)
{
  g_ArdOs.Mutex_lock(m_mutex);
  if (s > 0)
    {
      m_speed_virage = s;
    }
  else
    {
      m_speed_virage = SPEED_MAX_VIR;
    }
  g_ArdOs.Mutex_unlock(m_mutex);
}

/**---------------------------------
 * Private functions
 ---------------------------------*/

void
Navigation::compute_odom (float dxG, float dxD)
{
  float ds = (dxG + dxD) / 2;
  float dtheta = (dxD - dxG) / (2 * VOIE);

  m_pose.x += ds * cos (m_pose.h + dtheta);
  m_pose.y += ds * sin (m_pose.h + dtheta);
  m_pose.h += dtheta;

//  Serial.print ("odom: G+");
//  Serial.print (dxG);
//  Serial.print ("  D+");
//  Serial.println (dxG);
//  Serial.print ("odom: ");
//  Serial.print (m_x);
//  Serial.print (",");
//  Serial.print (m_y);
//  Serial.print (",");
//  Serial.println (m_t);
}

void
Navigation::straight (float mm)
{
  stepperG.setCurrentPosition (0);
  stepperD.setCurrentPosition (0);
  stepperG.setMaxSpeed (m_speed);
  stepperD.setMaxSpeed (m_speed);
  stepperG.moveTo (-mm * GAIN_STEP_MM);
  stepperD.moveTo (mm * GAIN_STEP_MM);
}

void
Navigation::turn (float angle)
{
  stepperG.setCurrentPosition (0);
  stepperD.setCurrentPosition (0);
  stepperG.setMaxSpeed (m_speed_virage);
  stepperD.setMaxSpeed (m_speed_virage);
  stepperG.moveTo (angle * VOIE / 2 * GAIN_STEP_MM);
  stepperD.moveTo (angle * VOIE / 2 * GAIN_STEP_MM);
}

void
Navigation::interruptCurrentMove ()
{
  LOG(INFO, "NAV : current order is interrupted.");
  m_state = eNavState::STOPPING;
  stepperG.stop ();
  stepperD.stop ();
}

/**
 * return true if the trajectory is finished
 */
bool
Navigation::subOrderFinished ()
{
  return (stepperG.distanceToGo () == 0 || stepperD.distanceToGo () == 0);
}

String
Navigation::sensToString (sens_t sens)
{
  switch (sens)
    {
    default:
    ENUM2STR(SENS_UNDEFINED)
;      ENUM2STR(SENS_AV);
      ENUM2STR(SENS_AR);
    }
}

String
Navigation::orderToString (eNavOrder order)
{
  switch (order)
    {
    default:
    ENUM2STR(eNavOrder::NOTHING)
;      ENUM2STR(eNavOrder::GOTO);
      ENUM2STR(eNavOrder::GOTO_CAP);
    }
}

String
Navigation::stateToString (eNavState state)
{
  switch (state)
    {
    default:
    ENUM2STR(eNavState::IDLE)
;      ENUM2STR(eNavState::FACING_DEST);
      ENUM2STR(eNavState::GOING_TO_TARGET);
      ENUM2STR(eNavState::TURNING_AT_TARGET);
      ENUM2STR(eNavState::STOPPING);
    }
}
