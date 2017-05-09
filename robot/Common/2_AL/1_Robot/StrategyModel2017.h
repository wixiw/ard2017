/*
 * StrategyModel2017.h
 *
 *  Created on: 19 avr. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_2_AL_0_STRATFWK_STRATEGYMODEL2017_H_
#define ROBOT_COMMON_2_AL_0_STRATFWK_STRATEGYMODEL2017_H_

#include "RSP.h"

namespace ard
{

//	On trouve le nommage des elements de jeu dans ARD_svn\1_Eurobot_2017\Plans Strat "Nommage elements table.png"

    class StrategyModel2017
    {
    public:
        StrategyModel2017();

        //Selects the match color
        void setColor(eColor color);
        eColor getColor();

        //cache to hold telemetry data
        apb_StratInfo2017 data;

        //Get the number of cylinders

        //Inform strategy that a cylinder has been withdrawn from our mono-color dispenser (A), 0 means all available
        void informWithdraw_A(uint8_t nb = 0);

        //Inform strategy that a cylinder has been withdrawn from our bi-color dispenser (G), 0 means all available
        void informWithdraw_G(uint8_t nb = 0);

        //Inform strategy that a cylinder has been withdrawn from opponent bi-color dispenser (Opp_G), 0 means all available
        void informWithdraw_OppG(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in the middle center container(zone 3), and decrease stock count, 0 means all available
        void informPooed_3(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in the middle container on our side (zone 4), and decrease stock count, 0 means all available
        void informPooed_4(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in the middle container on opponent side (zone 2), and decrease stock count, 0 means all available
        void informPooed_2(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in the border container on our side (zone 5), and decrease stock count, 0 means all available
        void informPooed_5(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in the border container on opponent side (zone Opp_5), and decrease stock count, 0 means all available
        void informPooed_1(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed in our start area (zone 6), and decrease stock count, 0 means all available
        void informPooed_6(uint8_t nb = 0);

        //Inform strategy that a cylinder has been pooed on table, 0 means all available
        void informPooed_OnTable(uint8_t nb = 0);

        //Inform strategy that a cylinder (D) has been taken on table (close to our start position)
        void informTaken_D();

        //Inform strategy that a cylinder (F) has been taken on table (close to our middle center container)
        void informTaken_F();

        //Inform strategy that a cylinder (E) has been taken on table (in the middle of our table side)
        void informTaken_E();

        //Inform strategy that a cylinder (B) has been taken on table (close to our start position table corner)
        void informTaken_B();

        //Inform strategy that a cylinder (C) has been taken on table (close to our bottom craters)
        void informTaken_C();

        //Inform strategy that a cylinder (Opp_D) has been taken on table (close to opponent start position)
        void informTaken_Opp_D();

        //Inform strategy that a cylinder (Opp_F) has been taken on table (close to opponent middle center container)
        void informTaken_Opp_F();

        //Inform strategy that a cylinder (Opp_E) has been taken on table (in the middle of opponent table side)
        void informTaken_Opp_E();

        //Inform strategy that a cylinder (D) has been push away from its default position (close to our start position)
        void informPushedAway_D();

        //Inform strategy that a cylinder (F) has been push away from its default position (close to our middle center container)
        void informPushedAway_F();

        //Inform strategy that a cylinder (E) has been push away from its default position (in the middle of our table side)
        void informPushedAway_E();

        //Inform strategy that a cylinder (B) has been push away from its default position (close to our start position table corner)
        void informPushedAway_B();

        //Inform strategy that a cylinder (C) has been push away from its default position (close to our bottom craters)
        void informPushedAway_C();

        //Inform strategy that a cylinder (Opp_D) has been push away from its default position (close to opponent start position)
        void informPushedAway_Opp_D();

        //Inform strategy that a cylinder (Opp_F) has been push away from its default position (close to opponent middle center container)
        void informPushedAway_Opp_F();

        //Inform strategy that a cylinder (Opp_E) has been push away from its default position (in the middle of opponent table side)
        void informPushedAway_Opp_E();

        //Used by telemetry to get fresh data
        apb_StratInfo2017 const& serialize();

    private:
        //helper function for updating stratInfo
        void withdraw(uint8_t nb, uint32_t& dispenserCount, eCylinderType cylType, String const& caller);
        void poo(uint8_t nb, uint8_t max, uint32_t& containerCount, uint8_t points, String const& caller);
        void take(bool& objectPresent, eCylinderType cylType, String const& caller);
        void push(bool& objectPresent, const String& caller);

    };

} /* namespace ard */

#endif /* ROBOT_COMMON_2_AL_0_STRATFWK_STRATEGYMODEL2017_H_ */
