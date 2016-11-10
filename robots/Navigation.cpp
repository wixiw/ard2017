#include <Arduino.h>
#include "LogThread.h"
#include "Navigation.h"
#include "K_constants.h"
#include "K_pinout.h"

using namespace ard;

Navigation::Navigation () :
    stepperG (AccelStepper::DRIVER, PAPG_STEP, PAPG_DIR), stepperD (
	AccelStepper::DRIVER, PAPD_STEP, PAPD_DIR)
{
  m_pose = PointCap ();
  m_target = PointCap ();
  m_sensTarget = SENS_UNDEFINED;
  m_color = COLOR_UNDEF;
  m_speed = SPEED_MAX;
  m_speed_virage = SPEED_MAX_VIR;
  m_state = IDLE;
  m_order = NOTHING;
  m_angleToTarget = 0;
  //TODO check completeness

  //TODO init des pins pinMode(PAP_ENABLE, OUTPUT);
}

void
Navigation::init ()
{
  m_mutex = g_ArdOs.Mutex_create();
  m_targetReached = g_ArdOs.Signal_create();
}

/**---------------------------------
 * Container thread interface
 ---------------------------------*/

void
Navigation::update (TimeMs sinceLastCall)
{
//  LOG(INFO, "update");

  stepperG.run ();
  stepperD.run ();
  //TODO compute_odom ?

  //Take a mutex to prevent localisation and target to be changed during a cycle
  g_ArdOs.Mutex_lock(m_mutex);

  switch (m_state)
    {
    default:
    case IDLE:
      {
	switch (m_order)
	  {
	  default:
	  case NOTHING:

	    break;
	  case GOTO:
	  case GOTO_CAP:
	    LOG(INFO,
		"NAV : new order " + orderToString (m_order) + "(" + m_target.x
		    + ", " + m_target.y + ", " + m_target.h + ") "
		    + sensToString (m_sensTarget) + ".");
	    if (m_sensTarget == SENS_AR)
	      m_angleToTarget = moduloPiPi (m_pose.angleTo (m_target) + M_PI);
	    else
	      m_angleToTarget = m_pose.angleTo (m_target);
	    turn (m_angleToTarget);
	    m_state = FACING_DEST;
	    break;
	  }
	break;
      }

    case FACING_DEST:
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
	    m_state = GOING_TO_TARGET;
	    LOG(DEBUG, "NAV : facing destination, beginning line.");
	  }
	break;
      }

    case GOING_TO_TARGET:
      {
	if (subOrderFinished ())
	  {
	    //Update odom
	    m_pose.x += m_distanceToTarget * cos (m_pose.h);
	    m_pose.y += m_distanceToTarget * sin (m_pose.h);

	    //Request rotation to final heading
	    if (m_order == GOTO_CAP)
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
	    m_state = TURNING_AT_TARGET;
	  }
	break;
      }

    case TURNING_AT_TARGET:
      {
	if (subOrderFinished ())
	  {
	    m_angleToTarget = 0;
	    m_state = IDLE;
	    LOG(INFO, "NAV : order finished.");
	    g_ArdOs.Signal_set(m_targetReached);
	  }
	break;
      }

    //TODO attention si on s'arrete avec ca, on ne compte pas le deplacement car c'est normalement fait a la fin
    case INTERRUPTING_ORDER:
      {
	if (subOrderFinished ())
	  {
	    m_state = IDLE;
	    LOG(INFO, "NAV : order interrupted.");
	  }
	break;
      }
  }

  g_ArdOs.Mutex_unlock(m_mutex);
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
  if (m_state != IDLE)
    {
      LOG(INFO, "NAV : new order pending until current order is finished");
      g_ArdOs.Mutex_unlock(m_mutex);
      wait ();
      g_ArdOs.Mutex_lock(m_mutex);
    }

  m_order = GOTO;
  m_target = target.toAmbiPose (m_color);
  m_sensTarget = sens;

  g_ArdOs.Mutex_unlock(m_mutex);
}

void
Navigation::goToCap (PointCap target, sens_t sens)
{
  g_ArdOs.Mutex_lock(m_mutex);

  //If an order is present, wait
  if (m_state != IDLE)
    {
      LOG(INFO, "NAV : new order pending until current order is finished");
      g_ArdOs.Mutex_unlock(m_mutex);
      wait ();
      g_ArdOs.Mutex_lock(m_mutex);
    }

  m_order = GOTO_CAP;
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
  if (m_state != IDLE)
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
  if (m_state != IDLE)
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
  if (m_state != IDLE)
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
Navigation::wait ()
{
  //obvisouly don't put a mutex, it's a blocking call ... !
  g_ArdOs.Signal_wait(m_targetReached);
}

bool
Navigation::targetReached ()
{
  g_ArdOs.Mutex_lock(m_mutex);
  bool res = m_state == IDLE;
  g_ArdOs.Mutex_unlock(m_mutex);
  return res;
}

/**---------------------------------
 * Nav configuration
 ---------------------------------*/

void
Navigation::setColor (color_t c)
{
  ardAssert(c != COLOR_UNDEF, "NAV : color should not be set to undefined.");
  g_ArdOs.Mutex_lock(m_mutex);
  m_color = c;
  g_ArdOs.Mutex_unlock(m_mutex);
  if (c == COLOR_PREF)
    LOG(INFO, "NAV : Color set to : PREF");
  else
    LOG(INFO, "NAV : Color set to : SYM");
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
  startTraj ();
  stepperG.setMaxSpeed (m_speed);
  stepperD.setMaxSpeed (m_speed);
  stepperG.moveTo (-mm * GAIN_STEP_MM);
  stepperD.moveTo (mm * GAIN_STEP_MM);
}

void
Navigation::turn (float angle)
{
  startTraj ();
  stepperG.setMaxSpeed (m_speed_virage);
  stepperD.setMaxSpeed (m_speed_virage);
  stepperG.moveTo (angle * VOIE / 2 * GAIN_STEP_MM);
  stepperD.moveTo (angle * VOIE / 2 * GAIN_STEP_MM);
}

void
Navigation::interrupt ()
{
  LOG(INFO, "NAV : current order is interrupted.");
  m_state = INTERRUPTING_ORDER;
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

void
Navigation::startTraj ()
{
  stepperG.setCurrentPosition (0);
  stepperD.setCurrentPosition (0);
  setSpeed (0);
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
    ENUM2STR(NOTHING)
;      ENUM2STR(GOTO);
      ENUM2STR(GOTO_CAP);
    }
}

String
Navigation::stateToString (eNavState state)
{
  switch (state)
    {
    default:
    ENUM2STR(IDLE)
;      ENUM2STR(FACING_DEST);
      ENUM2STR(GOING_TO_TARGET);
      ENUM2STR(TURNING_AT_TARGET);
      ENUM2STR(INTERRUPTING_ORDER);
    }
}
