/*
 * StrategyModel2017.cpp
 *
 *  Created on: 19 avr. 2017
 *      Author: wix
 */

#include "StrategyModel2017.h"


using namespace ard;

StrategyModel2017::StrategyModel2017()
{
    data = apb_StratInfo2017_init_default;
    data.matchColor                = eColor_UNKNOWN;
    data.dispenserMonocolorNb      = 4;
    data.dispenserBicolorNb        = 4;
    data.dispenserOppNb            = 4;
    data.cylinderStart             = true;
    data.cylinderContainer         = true;
    data.cylinderCenter            = true;
    data.cylinderCorner            = true;
    data.cylinderCrater            = true;
    data.cylinderOppStart          = true;
    data.cylinderOppContainer      = true;
    data.cylinderOppCenter         = true;
    data.robotCylinderStockNb      = 0;
    data.score                     = 0;
//TODO WIX : a remettre dans le proto
//    for(int i = 0 ; i < 6 ; i++)
//        data.stock[i] = eCylinderType_NO_CYLINDER;
}

void StrategyModel2017::setColor(eColor color)
{
    data.matchColor = color;
}

void StrategyModel2017::withdraw(uint8_t nb, uint32_t& dispenserCount, eCylinderType cylType, const String& caller)
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

    data.robotCylinderStockNb += nbTransfered;

    //Too many item in robot
    if( 6 < data.robotCylinderStockNb )
    {
        data.robotCylinderStockNb = 6;
        res = false;
    }
//TODO WIX
//    else
//    {
//        for(uint32_t i = 0; i < nbTransfered ; i++)
//        {
//            data.stock[data.robotCylinderStockNb-i] = cylType;
//        }
//    }

    String log = caller + "(" + nb + "): taken=" + nbTransfered + ", remaining : "
            + dispenserCount + "/4, robot stock : "
            + data.robotCylinderStockNb + "/6.";

    if(res)
    {
        LOG_INFO(log);
    }
    else
    {
        LOG_ERROR(log);
    }
}

void StrategyModel2017::poo(uint8_t nb, uint8_t max, uint32_t& containerCount, uint8_t points, const String& caller)
{
    uint32_t nbTransfered = nb;
    bool res = true;

    //Not enougth items in robot
    if( data.robotCylinderStockNb < nb )
    {
        nbTransfered = data.robotCylinderStockNb;
        res = false;
    }

    //Too many items for container
    if( max < containerCount + nbTransfered )
    {
        nbTransfered = max - containerCount;
        res = false;
    }

    for(uint32_t i = 0; i < nbTransfered ; i++)
    {
        //TODO WIX data.stock[data.robotCylinderStockNb - i] = eCylinderType_NO_CYLINDER;
        data.score += points;
    }

    containerCount += nbTransfered;
    data.robotCylinderStockNb -= nbTransfered;

    String log =caller + "(" + nb + "): pooed=" + nbTransfered + ", contained : " + containerCount + "/" + max
            + ", robot stock : " + data.robotCylinderStockNb + "/6.";

    if( res )
    {
        LOG_INFO(log);
    }
    else
    {
        LOG_ERROR(log);
    }
}

void StrategyModel2017::take(bool& objectPresent, eCylinderType cylType, const String& caller)
{
    uint32_t nbTransfered = 1;
    bool res = true;

    if( !objectPresent )
    {
        nbTransfered = 0;
        res = false;
    }

    objectPresent = false;
    data.robotCylinderStockNb += nbTransfered;

    //Too many item in robot
    if( 6 < data.robotCylinderStockNb )
    {
        data.robotCylinderStockNb = 6;
        res = false;
    }
    //TODO WIX
//    else
//    {
//        for(uint32_t i = 0; i < nbTransfered ; i++)
//        {
//            data.stock[data.robotCylinderStockNb-i] = cylType;
//        }
//    }

    String log = caller + ": taken=" + nbTransfered + ", robot stock : "
            + data.robotCylinderStockNb + "/6.";

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
        nb = data.dispenserMonocolorNb;
    }
    withdraw(nb, data.dispenserMonocolorNb, eCylinderType_MONOCOLOR, "informWithdraw_MonocolorDispenser");
}

void StrategyModel2017::informWithdraw_G(uint8_t nb)
{
    if(nb == 0)
    {
        nb = data.dispenserBicolorNb;
    }
    withdraw(nb, data.dispenserBicolorNb, eCylinderType_BICOLOR, "informWithdraw_BicolorDispenser");
}

void StrategyModel2017::informWithdraw_OppG(uint8_t nb)
{
    if(nb == 0)
    {
        nb = data.dispenserOppNb;
    }
    withdraw(nb, data.dispenserOppNb, eCylinderType_MONOCOLOR, "informWithdraw_OppDispenser");
}

void StrategyModel2017::informPooed_3(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - data.containerMidleCenterNb, data.robotCylinderStockNb);
    }
    poo(nb, 6, data.containerMidleCenterNb, 10, "informPooed_MiddleCenter");
}

void StrategyModel2017::informPooed_4(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - data.containerMidleOwnNb, data.robotCylinderStockNb);
    }
    poo(nb, 6, data.containerMidleOwnNb, 10, "informPooed_MiddleOwn");
}

void StrategyModel2017::informPooed_2(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - data.containerMidleOppNb, data.robotCylinderStockNb);
    }
    poo(nb, 6, data.containerMidleOppNb, 10, "informPooed_MiddleOpp");
}

void StrategyModel2017::informPooed_5(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(4 - data.containerBorderNb, data.robotCylinderStockNb);
    }
    poo(nb, 4, data.containerBorderNb, 10, "informPooed_Border");
}

void StrategyModel2017::informPooed_1(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(4 - data.containerBorderOppNb, data.robotCylinderStockNb);
    }
    poo(nb, 4, data.containerBorderOppNb, 10, "informPooed_BorderOpp");
}

void StrategyModel2017::informPooed_6(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(8 - data.containerStartNb, data.robotCylinderStockNb);
    }
    poo(nb, 8, data.containerStartNb, 2, "informPooed_Start");
}

void StrategyModel2017::informPooed_OnTable(uint8_t nb)
{
    if(nb == 0)
    {
        nb = data.robotCylinderStockNb;
    }
    uint32_t fakeCount = 0;
    poo(nb, 100, fakeCount, 0, "informPooed_OnTable");
}

void StrategyModel2017::informTaken_D()
{
    take( data.cylinderStart, eCylinderType_MONOCOLOR, "informTaken_Start");
}

void StrategyModel2017::informTaken_F()
{
    take( data.cylinderContainer, eCylinderType_MONOCOLOR, "informTaken_Container");
}

void StrategyModel2017::informTaken_E()
{
    take( data.cylinderCenter, eCylinderType_MONOCOLOR, "informTaken_Center");
}

void StrategyModel2017::informTaken_B()
{
    take( data.cylinderCorner, eCylinderType_MONOCOLOR, "informTaken_Corner");
}

void StrategyModel2017::informTaken_C()
{
    take( data.cylinderCrater, eCylinderType_MONOCOLOR, "informTaken_Crater");
}

void StrategyModel2017::informTaken_Opp_D()
{
    take( data.cylinderOppStart, eCylinderType_MONOCOLOR, "informTaken_OppStart");
}

void StrategyModel2017::informTaken_Opp_F()
{
    take( data.cylinderOppContainer, eCylinderType_MONOCOLOR, "informTaken_OppContainer");
}

void StrategyModel2017::informTaken_Opp_E()
{
    take( data.cylinderOppCenter, eCylinderType_MONOCOLOR, "informTaken_OppCenter");
}

void StrategyModel2017::informPushedAway_D()
{
    push( data.cylinderStart, "informPushedAway_Start");
}

void StrategyModel2017::informPushedAway_F()
{
    push( data.cylinderContainer, "informPushedAway_Container");
}

void StrategyModel2017::informPushedAway_E()
{
    push( data.cylinderCenter, "informPushedAway_Center");
}

void StrategyModel2017::informPushedAway_B()
{
    push( data.cylinderCorner, "informPushedAway_Corner");
}

void StrategyModel2017::informPushedAway_C()
{
    push( data.cylinderCrater, "informPushedAway_Crater");
}

void StrategyModel2017::informPushedAway_Opp_D()
{
    push( data.cylinderOppStart, "informPushedAway_OppStart");
}

void StrategyModel2017::informPushedAway_Opp_F()
{
    push( data.cylinderOppContainer, "informPushedAway_OppContainer");
}

void StrategyModel2017::informPushedAway_Opp_E()
{
    push( data.cylinderOppCenter, "informPushedAway_OppCenter");
}

apb_StratInfo2017 const& StrategyModel2017::serialize()
{
    return data;
}

