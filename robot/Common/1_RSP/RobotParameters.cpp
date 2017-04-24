/*
 * RobotConfig.cpp
 *
 *  Created on: 2 avr. 2017
 *      Author: wix
 */

#include "RobotParameters.h"

#include "K_constants.h"
#include "core/ArdMaths.h"
#include "Log.h"

using namespace ard;

RobotParameters::RobotParameters()
        :
                GAIN_MM_2_STEPS_LEFT(0.0),
                GAIN_MM_2_STEPS_RIGHT(0.0),
                GAIN_STEPS_2_MM_LEFT(0.0),
                GAIN_STEPS_2_MM_RIGHT(0.0),
                GAIN_RAD_2_STEPS_LEFT(0.0),
                GAIN_RAD_2_STEPS_RIGHT(0.0),
                cfg(),
                m_maxSpeed(0),
                m_configuredOnce(false)
{
    strcpy(cfg.serialNumber,    "");
    cfg.stepByTurn              = 1600;
    cfg.xav                     = 145;//mm
    cfg.xar                     = 30;//mm
    cfg.yside                   = 88;
    cfg.xavExtended             = 210;//mm
    cfg.leftWheelDiameter       = 60.000;
    cfg.rightWheelDiameter      = 60.000;
    cfg.voie                    = 160.4;
    cfg.maxAcc                  = 700;
    cfg.maxTurnAcc              = 300;
    cfg.recalSpeed              = 210.0;
    cfg.maxTurnSpeed            = 125.0;
    cfg.deccDist                = 150.0;
    cfg.strategyDuration        = 89500;
    cfg.detectionWaitForOppMove = 1000;
    cfg.detectionActive         = true;
}

void RobotParameters::setComputedVars()
{
    GAIN_MM_2_STEPS_LEFT = -(cfg.stepByTurn / (cfg.leftWheelDiameter * M_PI));
    GAIN_MM_2_STEPS_RIGHT = cfg.stepByTurn / (cfg.rightWheelDiameter * M_PI);
    GAIN_STEPS_2_MM_LEFT = 1 / GAIN_MM_2_STEPS_LEFT;
    GAIN_STEPS_2_MM_RIGHT = 1 / GAIN_MM_2_STEPS_RIGHT;
    GAIN_RAD_2_STEPS_LEFT = cfg.voie * GAIN_MM_2_STEPS_LEFT / 2.;
    GAIN_RAD_2_STEPS_RIGHT = cfg.voie * GAIN_MM_2_STEPS_RIGHT / 2.;
    m_maxSpeed = sqrt(2 * cfg.maxAcc * cfg.deccDist);
}

#define CHECK_RANGE(min, value, max) IS_OUT_RANGE(min, value, max) \
{ \
    LOG_ERROR(String(#value) + " is out of acceptable range [" + min + ", " +  max + "].");    \
    res &= false;   \
}

bool RobotParameters::checkConfig(apb_Configuration const& newConf)
{
    bool res = true;
    double newMaxSpeed = sqrt(2 * newConf.maxAcc * newConf.deccDist);

    //Checks :
    CHECK_RANGE(5, newConf.xav, 500);
    CHECK_RANGE(5, newConf.xar, 500);
    CHECK_RANGE(5, newConf.yside, 500);
    CHECK_RANGE(newConf.xav, newConf.xavExtended, 500);
    CHECK_RANGE(50, newConf.leftWheelDiameter, 70);
    CHECK_RANGE(50, newConf.rightWheelDiameter, 70);
    CHECK_RANGE(50, newConf.voie, 300);
    CHECK_RANGE(MINBOUND_MAXACC, newConf.maxAcc, MAXBOUND_MAXACC);
    CHECK_RANGE(MINBOUND_MAXTURNSPEED, newConf.maxTurnSpeed, MAXBOUND_MAXTURNSPEED);
    CHECK_RANGE(50, newConf.recalSpeed, 500);
    CHECK_RANGE(MINBOUND_MAXTURNACC, newConf.maxTurnAcc, MAXBOUND_MAXTURNACC);
    CHECK_RANGE(20, newConf.deccDist, 250);
    if (newConf.strategyDuration)
    {
        CHECK_RANGE(50000, newConf.strategyDuration, 89999);
    }
    CHECK_RANGE(50, newConf.detectionWaitForOppMove, 5000);
    CHECK_RANGE(MINBOUND_MAXSPEED, newMaxSpeed, MAXBOUND_MAXSPEED);

    if(strcmp(newConf.serialNumber, "") == 0)
    {
        LOG_ERROR("Serial number shall not be empty");
        res = false;
    }


    return res;
}

bool RobotParameters::setConfig(apb_Configuration const& newConf)
{
    //update config.
    if (checkConfig(newConf))
    {
        cfg = newConf;
        setComputedVars();
        m_configuredOnce = true;
        LOG_INFO("New configuration applied");
        return true;
    }
    else
    {
        LOG_ERROR("Configuration not applied");
        return false;
    }
}

uint32_t RobotParameters::stepByTurn() const
{
    ASSERT(m_configuredOnce);
    return cfg.stepByTurn;
}

uint32_t RobotParameters::xav() const
{
    ASSERT(m_configuredOnce);
    return cfg.xav;
}

uint32_t RobotParameters::xar() const
{
    ASSERT(m_configuredOnce);
    return cfg.xar;
}

uint32_t RobotParameters::yside() const
{
    ASSERT(m_configuredOnce);
    return cfg.yside;
}

uint32_t RobotParameters::xavExtended() const
{
    ASSERT(m_configuredOnce);
    return cfg.xavExtended;
}

float RobotParameters::leftWheelDiameter() const
{
    ASSERT(m_configuredOnce);
    return cfg.leftWheelDiameter;
}

float RobotParameters::rightWheelDiameter() const
{
    ASSERT(m_configuredOnce);
    return cfg.rightWheelDiameter;
}

float RobotParameters::voie() const
{
    ASSERT(m_configuredOnce);
    return cfg.voie;
}

uint32_t RobotParameters::maxAcc() const
{
    ASSERT(m_configuredOnce);
    return cfg.maxAcc;
}

uint32_t RobotParameters::maxTurnAcc() const
{
    ASSERT(m_configuredOnce);
    return cfg.maxTurnAcc;
}

uint32_t RobotParameters::recalSpeed() const
{
    ASSERT(m_configuredOnce);
    return cfg.recalSpeed;
}

uint32_t RobotParameters::maxTurnSpeed() const
{
    ASSERT(m_configuredOnce);
    return cfg.maxTurnSpeed;
}

uint32_t RobotParameters::deccDist() const
{
    ASSERT(m_configuredOnce);
    return cfg.deccDist;
}

uint32_t RobotParameters::strategyDuration() const
{
    ASSERT(m_configuredOnce);
    return cfg.strategyDuration;
}

uint32_t RobotParameters::detectionWaitForOppMove() const
{
    ASSERT(m_configuredOnce);
    return cfg.detectionWaitForOppMove;
}

bool RobotParameters::detectionActive() const
{
    ASSERT(m_configuredOnce);
    return cfg.detectionActive;
}

uint32_t RobotParameters::maxSpeed() const
{
    ASSERT(m_configuredOnce);
    return m_maxSpeed;
}

char const* const RobotParameters::serialNumber() const
{
    ASSERT(m_configuredOnce);
    return cfg.serialNumber;
}

void RobotParameters::set_stepByTurn(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.stepByTurn = value;
    setConfig(newConf);
}

void RobotParameters::set_xav(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.xav = value;
    setConfig(newConf);
}

void RobotParameters::set_xar(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.xar = value;
    setConfig(newConf);
}

void RobotParameters::set_yside(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.yside = value;
    setConfig(newConf);
}

void RobotParameters::set_xavExtended(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.xavExtended = value;
    setConfig(newConf);
}

void RobotParameters::set_leftWheelDiameter(float value)
{
    apb_Configuration newConf = cfg;
    newConf.leftWheelDiameter = value;
    setConfig(newConf);
}

void RobotParameters::set_rightWheelDiameter(float value)
{
    apb_Configuration newConf = cfg;
    newConf.rightWheelDiameter = value;
    setConfig(newConf);
}

void RobotParameters::set_voie(float value)
{
    apb_Configuration newConf = cfg;
    newConf.voie = value;
    setConfig(newConf);
}

void RobotParameters::set_maxAcc(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.maxAcc = value;
    setConfig(newConf);
}

void RobotParameters::set_maxTurnAcc(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.maxTurnAcc = value;
    setConfig(newConf);
}

void RobotParameters::set_recalSpeed(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.recalSpeed = value;
    setConfig(newConf);
}

void RobotParameters::set_maxTurnSpeed(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.maxTurnSpeed = value;
    setConfig(newConf);
}

void RobotParameters::set_deccDist(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.deccDist = value;
    setConfig(newConf);
}

void RobotParameters::set_strategyDuration(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.strategyDuration = value;
    setConfig(newConf);
}

void RobotParameters::set_detectionWaitForOppMove(uint32_t value)
{
    apb_Configuration newConf = cfg;
    newConf.detectionWaitForOppMove = value;
    setConfig(newConf);
}

void RobotParameters::set_detectionActive(bool value)
{
    apb_Configuration newConf = cfg;
    newConf.detectionActive = value;
    setConfig(newConf);
}
