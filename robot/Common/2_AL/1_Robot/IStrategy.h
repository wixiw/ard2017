/*
 * IStrategy.h
 *
 *  Created on: 26 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_0_STRATFWK_ISTRATEGY_H_
#define ROBOT_COMMON_2_AL_0_STRATFWK_ISTRATEGY_H_

#include "RSP.h"

namespace ard
{

    typedef enum
    {
        None,
        Success,
        Failed,
        InProgress,
    } StrategyResult;

    class IStrategy: public PolledObject
    {
    public:
        //Redefined to force the name
        IStrategy(String const& name):PolledObject(name), status(None){};

        virtual void start() = 0;
        virtual void stop() = 0;
        virtual StrategyResult getStatus(){return status;};
        virtual Pose2D getStartPosition(){return Pose2D();};

    protected:
        StrategyResult status;
    };

    /**
     * This interface is used to be able to get a pointer on the LSA
     * without having to get the template meta data issues
     */
    class LSA
    {
    public:
        virtual ~LSA() = default;

        //Get the expected position of the robot when the LSA begins
        virtual Pose2D const& getEntryPoint() const = 0;

        //Set the entry point
        virtual void setEntryPoint(Pose2D const& point) = 0;

        //design issue : users of LSA would like to poll it,
        //it's not possible to share the PolledObject interface
        //as it's also used in other class used in concrete LSA.
        //so to prevent loosing time as competition is approaching
        //this method replace the one of PolledObject and the child
        //class has to do the wrapping. Any idea welcome.
        virtual void updateLSA(DelayMs sinceLastCall) = 0;
        //Same remarks here
        virtual void startLSA() = 0;
        //Same remarks here
        virtual void stopLSA() = 0;
        //Same remarks here
        virtual StrategyResult getStatusLSA() = 0;

        //Get the nominal duration
        virtual DelayMs getNominalDuration() = 0;
    };
}
#endif /* ROBOT_COMMON_2_AL_0_STRATFWK_ISTRATEGY_H_ */
