// Classe de gestion de la navigation (déplacements du robot sur la table)
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "BSP.hpp"
#include "core/ArdMaths.h"
#include "Actuators/AccelStepper.h"
#include "CommonMsg.pb.h"
#include "RobotParameters.h"

namespace ard
{
    class Buzzer;

    /**
     * This class manage the robot position movements, and avoidance
     * It is isolated from the rest of the code to prevent something
     * to block the avoidance/localisation part.
     */
    class Navigation: public Thread
    {
    public:
        Navigation(Buzzer& klaxon);

        //Reread the configuration and maps default config. Shall be called at least once
        //before the OS is initialized
        void updateConf(RobotParameters* newConf);


        bool fakeRobot; //is true when a fake robot is simulated

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

        /**
         * Force a new robot position
         * the position may be automatically symetrized depending
         * on the value configured with setColor() and sym parameter
         */
        void setPosition(PointCap newPose, bool sym = true);
        void setPosition(float x/*mm*/, float y/*mm*/, float h/*deg*/, bool sym = true)
        {
            setPosition(PointCap(x, y, h));
        }
        ;

        /**
         * Set a new user speed/acc constraint
         */
        void setSpeedAcc(uint16_t vMax /*mm/s*/, uint16_t vMaxsTurn /*°/s*/,
                uint16_t accMax /*mm/s2*/, uint16_t accMaxTurn /*°/s2*/);

        /**
         * Get the current robot position
         */
        PointCap getPosition()
        {
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
        void goToCap(PointCap target, eDir sens = eDir_FORWARD, bool sym = true);
        void goToCap(float x/*mm*/, float y/*mm*/, float h/*°*/, eDir sens = eDir_FORWARD, bool sym = true)
        {
            goToCap(PointCap(x, y, h), sens, sym);
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

        /**
         * The robot will go to the following border and re localize accordingly
         */
        void recalFace(eTableBorder border);
        void recalRear(eTableBorder border);

        /**
         * Stops the robot. It interrupts current order.
         */
        void stopMoving();

        /**
         * This is a blocking call until the current order is finished
         */
        void wait();

        /**
         * This is the non blocking version of wait()
         */
        bool targetReached();

        /**
         * Active/Deactivate avoidance system
         */
        void enableAvoidance(bool on);

        /**---------------------------------
         * Nav configuration
         ---------------------------------*/

        void
        setColor(eColor c);

        /**---------------------------------
         * Publish state
         ---------------------------------*/

        apb_NavState const& serealize();

        //Avoidance sensors
        FilteredInput omronFrontLeft;
        FilteredInput omronFrontRight;
        FilteredInput omronRearLeft;
        FilteredInput omronRearRight;

        //Recal sensors
        FilteredInput switchRecalFL;
        FilteredInput switchRecalFR;
        FilteredInput switchRecalRC;

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
        PointCap getRecalPointFace(eTableBorder border);
        PointCap getRecalPointRear(eTableBorder border);

        String
        sensToString(eDir sens);
        String
        orderToString(eNavOrder order);
        String
        stateToString(eNavState state);

        //Test if opponent is ahead of robot
        bool isOpponentAhead();

        //Test if opponent is behind robot
        bool isOpponentBehind();

        //status
        PointCap m_pose; //critical section
        eNavState m_state;

        //target definition
        PointCap m_target;
        eDir m_sensTarget;
        eNavOrder m_order;

        //move constraints
        uint16_t userMaxSpeed;      //mm/s
        uint16_t userMaxTurnSpeed;  //°/s
        uint16_t userMaxAcc;        //mm/s²
        uint16_t userMaxTurnAcc;    //°/s²

        //HW interface
        AccelStepper stepperL; //critical section
        AccelStepper stepperR; //critical section

        //match color
        eColor m_color;

        Mutex m_mutex;

        long oldStepL; //critical section
        long oldStepR; //critical section

        //opponent management
        SwTimer oppTimer;
        bool avoidanceActive; //is true when avoidance system is active

        RobotParameters* conf;

        //recal managemement
        bool noSwitchMode;

        //for telemetry
        apb_NavState state;

        //klaxon to warn for failure and request opponent to move
        Buzzer& klaxon;
    };
}    //end namespace

#endif
