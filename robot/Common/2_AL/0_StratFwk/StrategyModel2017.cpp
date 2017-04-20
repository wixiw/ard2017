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

Strategy2017::Strategy2017():
        robot(NULL)
{

}

void Strategy2017::init()
{
    ASSERT_TEXT(robot, "You did not attached a robot.");
}

void Strategy2017::attachRobot(Robot2017* _robot)
{
    ASSERT_TEXT(robot==NULL, "You attached a robot twice.");
    robot = _robot;
}

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

void StrategyModel2017::informWithdraw_MonocolorDispenser(uint8_t nb)
{
    if(nb == 0)
    {
        nb = stratInfo.dispenserMonocolorNb;
    }
    withdraw(nb, stratInfo.dispenserMonocolorNb, "informWithdraw_MonocolorDispenser");
}

void StrategyModel2017::informWithdraw_BicolorDispenser(uint8_t nb)
{
    if(nb == 0)
    {
        nb = stratInfo.dispenserBicolorNb;
    }
    withdraw(nb, stratInfo.dispenserBicolorNb, "informWithdraw_BicolorDispenser");
}

void StrategyModel2017::informWithdraw_OppDispenser(uint8_t nb)
{
    if(nb == 0)
    {
        nb = stratInfo.dispenserOppNb;
    }
    withdraw(nb, stratInfo.dispenserOppNb, "informWithdraw_OppDispenser");
}

void StrategyModel2017::informPooed_MiddleCenter(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - stratInfo.containerMidleCenterNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 6, stratInfo.containerMidleCenterNb, "informPooed_MiddleCenter");
}

void StrategyModel2017::informPooed_MiddleOwn(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - stratInfo.containerMidleOwnNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 6, stratInfo.containerMidleOwnNb, "informPooed_MiddleOwn");
}

void StrategyModel2017::informPooed_MiddleOpp(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(6 - stratInfo.containerMidleOppNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 6, stratInfo.containerMidleOppNb, "informPooed_MiddleOpp");
}

void StrategyModel2017::informPooed_Border(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(4 - stratInfo.containerBorderNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 4, stratInfo.containerBorderNb, "informPooed_Border");
}

void StrategyModel2017::informPooed_BorderOpp(uint8_t nb)
{
    if(nb == 0)
    {
        nb = min(4 - stratInfo.containerBorderOppNb, stratInfo.robotCylinderStockNb);
    }
    poo(nb, 4, stratInfo.containerBorderOppNb, "informPooed_BorderOpp");
}

void StrategyModel2017::informPooed_Start(uint8_t nb)
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

void StrategyModel2017::informTaken_Start()
{
    take( stratInfo.cylinderStart, "informTaken_Start");
}

void StrategyModel2017::informTaken_Container()
{
    take( stratInfo.cylinderContainer, "informTaken_Container");
}

void StrategyModel2017::informTaken_Center()
{
    take( stratInfo.cylinderCenter, "informTaken_Center");
}

void StrategyModel2017::informTaken_Corner()
{
    take( stratInfo.cylinderCorner, "informTaken_Corner");
}

void StrategyModel2017::informTaken_Crater()
{
    take( stratInfo.cylinderCrater, "informTaken_Crater");
}

void StrategyModel2017::informTaken_OppStart()
{
    take( stratInfo.cylinderOppStart, "informTaken_OppStart");
}

void StrategyModel2017::informTaken_OppContainer()
{
    take( stratInfo.cylinderOppContainer, "informTaken_OppContainer");
}

void StrategyModel2017::informTaken_OppCenter()
{
    take( stratInfo.cylinderOppCenter, "informTaken_OppCenter");
}

void StrategyModel2017::informPushedAway_Start()
{
    push( stratInfo.cylinderStart, "informPushedAway_Start");
}

void StrategyModel2017::informPushedAway_Container()
{
    push( stratInfo.cylinderContainer, "informPushedAway_Container");
}

void StrategyModel2017::informPushedAway_Center()
{
    push( stratInfo.cylinderCenter, "informPushedAway_Center");
}

void StrategyModel2017::informPushedAway_Corner()
{
    push( stratInfo.cylinderCorner, "informPushedAway_Corner");
}

void StrategyModel2017::informPushedAway_Crater()
{
    push( stratInfo.cylinderCrater, "informPushedAway_Crater");
}

void StrategyModel2017::informPushedAway_OppStart()
{
    push( stratInfo.cylinderOppStart, "informPushedAway_OppStart");
}

void StrategyModel2017::informPushedAway_OppContainer()
{
    push( stratInfo.cylinderOppContainer, "informPushedAway_OppContainer");
}

void StrategyModel2017::informPushedAway_OppCenter()
{
    push( stratInfo.cylinderOppCenter, "informPushedAway_OppCenter");
}

apb_StratInfo2017 const& StrategyModel2017::getStratInfo()
{
    return stratInfo;
}

#endif
