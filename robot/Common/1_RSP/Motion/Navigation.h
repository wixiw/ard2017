// Classe de gestion de la navigation (déplacements du robot sur la table)
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "BSP.hpp"
#include "core/ArdMaths.h"
#include "Actuators/AccelStepper.h"
#include "CommonMsg.pb.h"
#include "RobotParameters.h"

//used for configuring recal
#define NO_ESCAPE true
#define RECAL_ESCAPE_MARGIN 30

namespace ard
{
    class Buzzer;
    class OppDetection;
    class Graph;
    class KinematicManager;

    /**
     * This class manage the robot position movements, and avoidance
     * It is isolated from the rest of the code to prevent something
     * to block the avoidance/localisation part.
     */
    class Navigation: public Thread, public RobotParametersListener
    {
    public:
        Navigation(Buzzer& klaxon, OppDetection& detection,
        		Graph& graph, KinematicManager& kinMan);

        /**---------------------------------
         * Thread interface
         ---------------------------------*/

        void init() override;

        //Implements Thread
        void run() override;

        //As the motor lib needs to be called
        void updateFromInterrupt();

        /**---------------------------------
         * User (= strategy) interface
         ---------------------------------*/

        //Activate or disable power on motors.
        void motorPower(bool on);

        /**
         * Force a new robot position
         * the position may be automatically symetrized depending
         * on the value configured with setColor() and sym parameter
         */
        void setPosition(Pose2D newPose, bool sym = true);
        void setPosition(float x/*mm*/, float y/*mm*/, float h/*deg*/, bool sym = true)
        {
            setPosition(Pose2D(x, y, h));
        }
        ;

        /**
         * Get the current robot position
         */
        Pose2D getPosition(bool sym = true) const
        {
        	if(sym)
        		return m_pose.toAmbiPose(m_color);
        	else
        		return m_pose;
        }

        /**
         Send a Goto (x,y) request, the robot will turn to face the target
         and use a straight line to go to it.
         If an order is already present the call is blocking as if a wait() where done.

         Note that the target may be automatically symetrized depending
         on the color configured with setColor() and the sym param

         x,y in mm

         */
        void goTo(Point target, eDir sens = eDir_FORWARD, bool sym = true);
        void goTo(float x /*mm*/, float y/*mm*/, eDir sens = eDir_FORWARD, bool sym = true)
        {
            goTo(Point(x, y), sens, sym);
        }
        ;

        /**
         Send a Goto (x,y,heading) request, the robot will turn to face the target
         and use a straight line to go to it. Then it will turn to reach the
         target heading/
         If an order is already present the call is blocking as if a wait() where done.

         Note that the target may be automatically symetrized depending
         on the color configured with setColor() and the sym param

         In order to wait until the order is complete, use the wait()/targetReached() functions

         x,y in mm
         heading in degrees

         */
        void goToCap(Pose2D target, eDir sens = eDir_FORWARD, bool sym = true);
        void goToCap(float x/*mm*/, float y/*mm*/, float h/*°*/, eDir sens = eDir_FORWARD, bool sym = true)
        {
            goToCap(Pose2D(x, y, h), sens, sym);
        }
        ;

        /**
         * The robot will go ahead of the distance in parameter
         * note : the distance may be negative to go rear-way
         */
        void goForward(float distanceMm);

        /**
         * The robot will turn relatively of the angle in parameter
         * note : the angle may be negative to go clockwise
         *
         * Note that the target may be automatically symetrized depending
         * on the color configured with setColor() and the sym param
         */
        void turnDelta(float angle/*deg*/, bool sym = true);

        /**
         * The robot will turn to face the parametered heading (absolute)
         *
         * Note that the target may be automatically symetrized depending
         * on the color configured with setColor() and the sym param
         */
        void turnTo(float angle/*deg*/, bool sym = true);

        /**
         * The robot will turn to face the point in parameter
         * Note that the target is automatically symetrized depending
         * on the color configured with setColor()
         *
         * Note that the target may be automatically symetrized depending
         * on the color configured with setColor() and the sym param
         */
        void faceTo(Point p, bool sym = true);
        void faceTo(float x/*mm*/, float y/*mm*/, bool sym = true)
        {
            faceTo(Point(x,y), sym);
        }
        void rearTo(Point p, bool sym = true);
        void rearTo(float x/*mm*/, float y/*mm*/, bool sym = true)
        {
            rearTo(Point(x,y), sym);
        }

        /**
         * The robot will go to the following border and re localize accordingly
         * @param escapeDir : the distance to escape from the wall
         */
        void recalFace(eTableBorder border, Distance escapeDir = RECAL_ESCAPE_MARGIN);
        void recalRear(eTableBorder border, Distance escapeDir = RECAL_ESCAPE_MARGIN);

        /**
         * Reach a target destination running in the graph
         */
        void graphTo(Pose2D target, eDir sens = eDir_BEST);
        void graphTo(float x/*mm*/, float y/*mm*/, float h, eDir sens = eDir_BEST)
        {
            graphTo(Pose2D(x,y,h), sens);
        }

        /**
         * Stops the robot. It interrupts current order.
         */
        void stopMoving(eNavState targetState = eNavState_STOPPING);

        /**
         * This is a blocking call until the current order is finished
         */
        void wait();

        /**
         * Returns true when the order is finished sucessfully
         */
        bool targetReached();

        /**
         * Returns true when the order is failed (either due to timeout or opponent presence)
         */
        bool blocked();

        /**---------------------------------
         * Nav configuration
         ---------------------------------*/

        void setColor(eColor c);

        //Compute the expected duration of a move from start point to target point, going in direction "sens" and turning at target if isGoto is true
        DelayMs motionDuration(Pose2D const& start, Pose2D const& end, eDir sens, bool isGotoCap);

        //Find the best direction (minimizing travel time) from start point to target point, turning at target if isGoto is true
        //duration is an optional output to get the best travel duration
        eDir findOptimalDir(Pose2D const& start, Pose2D const& end, bool isGotoCap, DelayMs* duration = NULL);

        //Find the escape dir distance depending on recel direction
        eDir computeRecalEscapdeDir();

        /**---------------------------------
         * Publish state
         ---------------------------------*/

        apb_NavState const& serealize();

        //Recal sensors
        FilteredInput switchRecalFL;
        FilteredInput switchRecalFR;
        FilteredInput switchRecalRC;

        //ignore inputs when simulated
        bool simulated;

    protected:
        void recalSwitchContacted(bool contactOk, double escapeDist);

    private:
        //Integrates the new displacement mesures with current position
        //This function modifies critical section variables with no protection
        //It's the caller responsibility to protect the call with portENTER_CRITICAL/portEXIT_CRITICAL from a thread, or call it in an interrupt context
        void
        compute_odom();

        //State machine transition actions
        void action_startOrder();
        void action_goingToTarget();
        void action_turningAtTarget();
        void action_gotoNextWaypoint();
        void action_finishOrder();
        void action_waitOppMove();

        //used to send a straight line trajectory to the motors, it's a relative order
        void applyCmdToGoStraight(double distInMm, double maxSpeed, double maxAcc);

        //used to send an on place rotation trajectory to the motors, its a relative order
        void applyCmdToTurn(double angleInRad, double maxSpeed, double maxAcc);

        //interrupt the current movement
        void interruptCurrentMove();

        //used internally after a straight/turn/face order to check completeness
        bool subOrderFinished();

        //Compute the point at which the recal shall set the new robot position
        Pose2D getRecalPointFace(eTableBorder border);
        Pose2D getRecalPointRear(eTableBorder border);

        String
        sensToString(eDir sens);
        String
        orderToString(eNavOrder order);
        String
        stateToString(eNavState state);

        //status
        Pose2D m_pose; //critical section
        eNavState m_state;

        //target definition
        Pose2D m_target;
        eDir m_targetDir;
        eNavOrder m_order;

        //HW interface
        AccelStepper stepperL; //critical section
        AccelStepper stepperR; //critical section

        //match color
        eColor m_color;

        Mutex m_mutex;

        long oldStepL; //critical section
        long oldStepR; //critical section

        //opponent management
        SwTimer orderTimeout;

        //GraphTo management
        uint8_t currentWayPoint;
        eDir m_graphDir;

        //for telemetry
        apb_NavState state;

        //Recal management
        Distance escapeDist;
        uint8_t triedCount;

        //klaxon to warn for failure and request opponent to move
        Buzzer& klaxon;
        OppDetection& detection;
        Graph& graph;
        KinematicManager& kinematics;
    };
}    //end namespace

#endif
