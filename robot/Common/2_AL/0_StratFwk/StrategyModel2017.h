/*
 * StrategyModel2017.h
 *
 *  Created on: 19 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_0_STRATFWK_STRATEGYMODEL2017_H_
#define ROBOT_COMMON_2_AL_0_STRATFWK_STRATEGYMODEL2017_H_

#include "RSP.h"

#ifdef BUILD_STRATEGY

#include "0_StratFwk/StratInterfaces.h"

namespace ard
{
    class Robot2017; //forward declare

    /**
     * This class contains a standard robot actuators/sensors interface
     * so that each build strategy doesn't have to redefine its own
     */
    class Strategy2017: public IStrategy
    {
    public:
        Strategy2017();

        //Overrides IStrategy
        virtual void init() override;

        void attachRobot(Robot2017* robot);
        void enableAvoidance(bool on);

    protected:
        Robot2017* robot;
    };

    class StrategyModel2017
    {
    public:
        StrategyModel2017();

        //Selects the match color
        void setColor(eColor color);

        //Inform strategy that a cylinder has been withdrawn from our mono-color dispenser, 0 means all available
        void informWithdraw_MonocolorDispenser(uint8_t nb = 0);

        //Inform strategy that a cylinder has been withdrawn from our bi-color dispenser, 0 means all available
        void informWithdraw_BicolorDispenser(uint8_t nb = 0);

        //Inform strategy that a cylinder has been withdrawn from opponent bi-color dispenser, 0 means all available
        void informWithdraw_OppDispenser(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in the middle center container, and decrease stock count, 0 means all available
        void informPooed_MiddleCenter(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in the middle container on our side, and decrease stock count, 0 means all available
        void informPooed_MiddleOwn(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in the middle container on opponent side, and decrease stock count, 0 means all available
        void informPooed_MiddleOpp(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in the border container on our side, and decrease stock count, 0 means all available
        void informPooed_Border(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in the border container on opponent side, and decrease stock count, 0 means all available
        void informPooed_BorderOpp(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in our start area, and decrease stock count, 0 means all available
        void informPooed_Start(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed on table, 0 means all available
        void informPooed_OnTable(uint8_t nb = 0);

        //Inform stategy that a cylinder has been taken on table (close to our start position)
        void informTaken_Start();

        //Inform stategy that a cylinder has been taken on table (close to our midle center container)
        void informTaken_Container();

        //Inform stategy that a cylinder has been taken on table (in the middle of our table side)
        void informTaken_Center();

        //Inform stategy that a cylinder has been taken on table (close to our start position table corner)
        void informTaken_Corner();

        //Inform stategy that a cylinder has been taken on table (close to our bottom craters)
        void informTaken_Crater();

        //Inform stategy that a cylinder has been taken on table (close to opponent start position)
        void informTaken_OppStart();

        //Inform stategy that a cylinder has been taken on table (close to opponent midle center container)
        void informTaken_OppContainer();

        //Inform stategy that a cylinder has been taken on table (in the middle of opponent table side)
        void informTaken_OppCenter();

        //Inform stategy that a cylinder has been push away from its default position (close to our start position)
        void informPushedAway_Start();

        //Inform stategy that a cylinder has been push away from its default position (close to our midle center container)
        void informPushedAway_Container();

        //Inform stategy that a cylinder has been push away from its default position (in the middle of our table side)
        void informPushedAway_Center();

        //Inform stategy that a cylinder has been push away from its default position (close to our start position table corner)
        void informPushedAway_Corner();

        //Inform stategy that a cylinder has been push away from its default position (close to our bottom craters)
        void informPushedAway_Crater();

        //Inform stategy that a cylinder has been push away from its default position (close to opponent start position)
        void informPushedAway_OppStart();

        //Inform stategy that a cylinder has been push away from its default position (close to opponent midle center container)
        void informPushedAway_OppContainer();

        //Inform stategy that a cylinder has been push away from its default position (in the middle of opponent table side)
        void informPushedAway_OppCenter();

        //Used by telemetry to get fresh data
        apb_StratInfo2017 const& getStratInfo();

    private:
        //helper function for updating stratInfo
        void withdraw(uint8_t nb, uint32_t& dispenserCount, String const& caller);
        void poo(uint8_t nb, uint8_t max, uint32_t& containerCount, String const& caller);
        void take(bool& objectPresent, String const& caller);
        void push(bool& objectPresent, const String& caller);

        //cache to hold telemetry data
        apb_StratInfo2017 stratInfo;
    };

} /* namespace ard */

#endif //build strategy

#endif /* ROBOT_COMMON_2_AL_0_STRATFWK_STRATEGYMODEL2017_H_ */
