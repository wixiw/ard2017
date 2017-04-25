/*
 * StrategyModel2017.cpp
 *
 *  Created on: 19 avr. 2017
 *      Author: wix
 */

#include "StrategyModel2017.h"
#include "Robot2017.h"

#ifdef BUILD_STRATEGY

using namespace ard;

StrategyModel2017::StrategyModel2017()
{
    stratInfo = apb_StratInfo2017_init_default;
    stratInfo.matchColor                = eColor_UNKNOWN;
    stratInfo.dispenserMonocolorNb      = 4;
    stratInfo.dispenserBicolorNb        = 4;
    stratInfo.dispenserOppNb            = 4;
    stratInfo.cylinderStart             = true;
    stratInfo.cylinderContainer         = true;
    stratInfo.cylinderCenter            = true;
    stratInfo.cylinderCorner            = true;
    stratInfo.cylinderCrater            = true;
    stratInfo.cylinderOppStart          = true;
    stratInfo.cylinderOppContainer      = true;
    stratInfo.cylinderOppCenter         = true;
    stratInfo.robotCylinderStockNb      = 0;
}

void StrategyModel2017::setColor(eColor color)
{
    stratInfo.matchColor = color;
}

void StrategyModel2017::withdraw(uint8_t nb, uint32_t& dispenserCount, const String& caller)
{
    uint32_t nbTransfered = 0;
    bool res = true;

    //nominal case
    if( nb <= dispenserCount )
    {
        nbTransfered = nb;
        dispenserCount -= nb;
    }
    //dispenser doesn't have enougth objects
    else
    {
        nbTransfered = dispenserCount;
        dispenserCount = 0;
        res = false;
    }

    stratInfo.robotCylinderStockNb += nbTransfered;

    //Too many item in robot
    if( 6 < stratInfo.robotCylinderStockNb )
    {
        stratInfo.robotCylinderStockNb = 6;
        res = false;
    }

    String log = caller + "(" + nb + "): taken=" + nbTransfered + ", remaining : "
            + dispenserCount + "/4, robot stock : "
            + stratInfo.robotCylinderStockNb + "/6.";

    if(res)
    {
        LOG_INFO(log);
    }
    else
    {
        LOG_ERROR(log);
    }
}

void StrategyModel2017::poo(uint8_t nb, uint8_t max, uint32_t& containerCount, const String& caller)
{
    uint32_t nbTransfered = nb;
    bool res = true;

    //Not enougth items in robot
    if( stratInfo.robotCylinderStockNb < nb )
    {
        nbTransfered = stratInfo.robotCylinderStockNb;
        res = false;
    }

    //Too many items for container
    if( max < containerCount + nbTransfered )
    {
        nbTransfered = max - containerCount;
        res = false;
    }

    containerCount += nbTransfered;
    stratInfo.robotCylinderStockNb -= nbTransfered;

    String log =caller + "(" + nb + "): pooed=" + nbTransfered + ", contained : " + containerCount + "/" + max
            + ", robot stock : " + stratInfo.robotCylinderStockNb + "/6.";

    if( res )
    {
        LOG_INFO(log);
    }
    else
    {
        LOG_ERROR(log);
    }
}

void StrategyModel2017::take(bool& objectPresent, const String& caller)
{
    uint32_t nbTransfered = 1;
    bool res = true;

    if( !objectPresent )
    {
        nbTransfered = 0;
        res = false;
    }

    objectPresent = false;
    stratInfo.robotCylinderStockNb += nbTransfered;

    //Too many item in robot
    if( 6 < stratInfo.robotCylinderStockNb )
    {
        stratInfo.robotCylinderStockNb = 6;
        res = false;
    }

    String log = caller + ": taken=" + nbTransfered + ", robot stock : "
            + stratInfo.robotCylinderStockNb + "/6.";

    if(res)
    {
        LOG_INFO(log);
    }
    else
    {
        LOG_ERROR(log);
    }
}

void StrategyModel2017::push(bool& objectPresent, const String& caller)
{
    objectPresent = false;
    LOG_INFO(caller);
}

void StrategyModel2017::informWithdraw_A(uint8_t nb)
{
    if(nb == 0)
    {
        nb = stratInfo.dispenserMonocolorNb;
    }
    withdraw(nb, stratInfo.dispenserMonocolorNb, "informWithdraw_MonocolorDispenser");
}

void StrategyModel2017::informWithdraw_G(uint8_t nb)
{
    if(nb == 0)
    {
        nb = stratInfo.dispenserBicolorNb;
    }
    withdraw(nb, stratInfo.dispenserBicolorNb, "informWithdraw_BicolorDispenser");
}

void StrategyModel2017::informWithdraw_Opp_G(uint8_t nb)
{
    if(nb == 0)
    {
        nb = stratInfo.dispenserOppNb;
    }
    withdraw(nb, stratInfo.dispenserOppNb, "informWithdraw_OppDispenser");
}

void StrategyModel2017::informPooed_3(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - stratInfo.containerMidleCenterNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 6, stratInfo.containerMidleCenterNb, "informPooed_MiddleCenter");
}

void StrategyModel2017::informPooed_4(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - stratInfo.containerMidleOwnNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 6, stratInfo.containerMidleOwnNb, "informPooed_MiddleOwn");
}

void StrategyModel2017::informPooed_2(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - stratInfo.containerMidleOppNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 6, stratInfo.containerMidleOppNb, "informPooed_MiddleOpp");
}

void StrategyModel2017::informPooed_5(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(4 - stratInfo.containerBorderNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 4, stratInfo.containerBorderNb, "informPooed_Border");
}

void StrategyModel2017::informPooed_Opp_5(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(4 - stratInfo.containerBorderOppNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 4, stratInfo.containerBorderOppNb, "informPooed_BorderOpp");
}

void StrategyModel2017::informPooed_6(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(8 - stratInfo.containerStartNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 8, stratInfo.containerStartNb, "informPooed_Start");
}

void StrategyModel2017::informPooed_OnTable(uint8_t nb)
{
    if(nb == 0)
    {
        nb = stratInfo.robotCylinderStockNb;
    }
    uint32_t fakeCount = 0;
    poo(nb, 100, fakeCount, "informPooed_OnTable");
}

void StrategyModel2017::informTaken_D()
{
    take( stratInfo.cylinderStart, "informTaken_Start");
}

void StrategyModel2017::informTaken_F()
{
    take( stratInfo.cylinderContainer, "informTaken_Container");
}

void StrategyModel2017::informTaken_E()
{
    take( stratInfo.cylinderCenter, "informTaken_Center");
}

void StrategyModel2017::informTaken_B()
{
    take( stratInfo.cylinderCorner, "informTaken_Corner");
}

void StrategyModel2017::informTaken_C()
{
    take( stratInfo.cylinderCrater, "informTaken_Crater");
}

void StrategyModel2017::informTaken_Opp_D()
{
    take( stratInfo.cylinderOppStart, "informTaken_OppStart");
}

void StrategyModel2017::informTaken_Opp_F()
{
    take( stratInfo.cylinderOppContainer, "informTaken_OppContainer");
}

void StrategyModel2017::informTaken_Opp_E()
{
    take( stratInfo.cylinderOppCenter, "informTaken_OppCenter");
}

void StrategyModel2017::informPushedAway_D()
{
    push( stratInfo.cylinderStart, "informPushedAway_Start");
}

void StrategyModel2017::informPushedAway_F()
{
    push( stratInfo.cylinderContainer, "informPushedAway_Container");
}

void StrategyModel2017::informPushedAway_E()
{
    push( stratInfo.cylinderCenter, "informPushedAway_Center");
}

void StrategyModel2017::informPushedAway_B()
{
    push( stratInfo.cylinderCorner, "informPushedAway_Corner");
}

void StrategyModel2017::informPushedAway_C()
{
    push( stratInfo.cylinderCrater, "informPushedAway_Crater");
}

void StrategyModel2017::informPushedAway_Opp_D()
{
    push( stratInfo.cylinderOppStart, "informPushedAway_OppStart");
}

void StrategyModel2017::informPushedAway_Opp_F()
{
    push( stratInfo.cylinderOppContainer, "informPushedAway_OppContainer");
}

void StrategyModel2017::informPushedAway_Opp_E()
{
    push( stratInfo.cylinderOppCenter, "informPushedAway_OppCenter");
}

apb_StratInfo2017 const& StrategyModel2017::getStratInfo()
{
    return stratInfo;
}

#endif
