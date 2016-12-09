// Classe de gestion de la navigation (déplacements du robot sur la table)
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "BSP.h"
#include "RSP.h"

namespace ard
{

  typedef enum
  {
    SENS_UNDEFINED = 0, SENS_AV = 1, SENS_AR = -1
  } sens_t;

  /**
   * This class manage the robot position and movements.
   * It is expected to be embedded in a periodic thread
   * Hence, the strategy will access it from another thread
   * Thus, introducing thread safety issue (so a mutex is present)
   */
  class Navigation : public IMiniPeriodicThread
  {
  public:
    Navigation ();

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

    //As the motor lib needs to be called
    void
    updateFromInterrupt();

    /**---------------------------------
     * User (= strategy) interface
     ---------------------------------*/

    /**
     * Force a new robot position
     * the position is automatically symetrized depending
     * on the value configured with setColor()
     */
    void
    setPosition (PointCap newPose);
    void
    setPosition(float x, float y, float h){setPosition(PointCap(x,y,h));};

    /**
     * Get the current robot position
     */
    PointCap
    getPosition ()
    {
      return m_pose;
    }

    /**
     Send a Goto (x,y) request, the robot will turn to face the target
     and use a straight line to go to it.
     If an order is already present the call is blocking as if a wait() where done.

     Note that the target is automatically symetrized depending
     on the color configured with setColor()

     In order to wait until the order is complete, use the wait()/targetReached() functions
     */
    void
    goTo (Point target, sens_t sens = SENS_AV);
    void
    goTo (float x, float y, sens_t sens = SENS_AV){goTo(Point(x,y), sens);};

    /**
     Send a Goto (x,y) request, the robot will turn to face the target
     and use a straight line to go to it. Then it will turn to reach the
     target heading/
     If an order is already present the call is blocking as if a wait() where done.

     Note that the target is automatically symetrized depending
     on the color configured with setColor()

     In order to wait until the order is complete, use the wait()/targetReached() functions
     */
    void
    goToCap (PointCap target, sens_t sens = SENS_AV);
    void
    goToCap(float x, float y, float h, sens_t sens = SENS_AV){goToCap(PointCap(x,y,h), sens);};

    /**
     * The robot will go ahead of the distance in parameter
     * note : the distance may be negative to go rear-way
     *
     * If an order is already present the call is blocking as if a wait() where done.
     */
    void
    goForward (float distanceMm);

    /**
     * The robot will turn of the angle in parameter
     * note : the angle may be negative to go clockwise
     *
     * If an order is already present the call is blocking as if a wait() where done.
     */
    void
    turnTo (float angle);

    /**
     * The robot will turn to face the point in parameter
     * Note that the target is automatically symetrized depending
     * on the color configured with setColor()
     *
     * If an order is already present the call is blocking as if a wait() where done.
     */
    void
    faceTo (Point p);

    /**
     * Stops the robot. It interrupts current order.
     */
    void 
    stop();

    /**
     * This is a blocking call until the current order is finished
     */
    void
    wait ();

    /**
     * This is the non blocking version of wait()
     */
    bool
    targetReached ();

    /**---------------------------------
     * Nav configuration
     ---------------------------------*/

    void
    setColor (eColor c);
    void
    setSpeed (float speed);
    void
    setSpeedVir (float s);

    eGpioLevel getOmronState_FL(){return omronFrontLeft.readRaw();};
    eGpioLevel getOmronState_FR(){return omronFrontRight.readRaw();};
    eGpioLevel getOmronState_RL(){return omronRearLeft.readRaw();};
    eGpioLevel getOmronState_RR(){return omronRearRight.readRaw();};

  private:
    typedef enum class eNavState
    {
      IDLE, FACING_DEST, GOING_TO_TARGET, TURNING_AT_TARGET, STOPPING
    } eNavState;

    typedef enum class eNavOrder
    {
      NOTHING, GOTO, GOTO_CAP, STOP
    } eNavOrder;

    //Integrates the new displacement mesures with current position
    void
    compute_odom (float dxG, float dxD);

    //used to send a straight line trajectory to the motors
    void
    straight (float distInMm);

    //used to send an on place rotation trajectory to the motors
    void
    turn (float angle);

    //interrupt the current movement
    void interruptCurrentMove();

    //used internally after a straight/turn/face order to check completeness
    bool
    subOrderFinished ();

    String
    sensToString (sens_t sens);
    String
    orderToString (eNavOrder order);
    String
    stateToString (eNavState state);

    //status
    PointCap m_pose;
    eNavState m_state;

    //target definition
    PointCap m_target;
    sens_t m_sensTarget;
    eNavOrder m_order;
    float m_angleToTarget;
    float m_distanceToTarget;

    //HW interface
    AccelStepper stepperG;
    AccelStepper stepperD;
    FilteredInput omronFrontLeft;
    FilteredInput omronFrontRight;
    FilteredInput omronRearLeft;
    FilteredInput omronRearRight;

    //match color
    eColor m_color;

    //speed is reduced in turns to prevent drifting
    //hence we need 2 vars to switch from one to the other
    float m_speed;
    float m_speed_virage;

    Mutex m_mutex;
    Signal m_targetReached;
  };
}    //end namespace

#endif
