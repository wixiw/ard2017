/*
 * StrategyThread.cpp
 *
 *  Created on: 20 oct. 2016
 *      Author: wix
 */

#include "Lifecycle.h"
#include "Robot2017.h"

using namespace ard;

Lifecycle::Lifecycle(Navigation& nav, Chrono& chrono, HmiThread& hmi, OppDetection& detection):
        Thread("Strategy", PRIO_STRATEGY, STACK_STRATEGY, PERIOD_STRATEGY),
        strategyId(0),
        nbRegisteredStrats(0),
        listener(NULL),
        selftest(NULL),
        nav(nav),
        chrono(chrono),
        hmi(hmi),
        detection(detection),
        currentMode(MODE_NONE),
        currentModeStatus(0),
        matchColor(eColor_UNKNOWN),
        simulated(true)
{
    fsm.setDefaultSCI_OCB(this);
    fsm.setTimer(&fsmTimer);
    INIT_TABLE_TO_ZERO(matchs);
    currentModeStatus = fsm.get_nONE();
}

//Dependency injection
void Lifecycle::attach(LifecycleListener* client)
{
    ASSERT_TEXT(client,"You should not attach a NULL robot");
    listener = client;
}

void Lifecycle::init()
{
    ASSERT(nbRegisteredStrats);
    Thread::init();
    fsm.init();
    fsm.enter();
    
    ASSERT(MODE_NONE        == fsm.get_mODE_NONE());
    ASSERT(MODE_SELFTEST    == fsm.get_mODE_SELFTEST());
    ASSERT(MODE_INSTALL     == fsm.get_mODE_INSTALL());
    ASSERT(MODE_CORE_MATCH  == fsm.get_mODE_CORE_MATCH());
    ASSERT(MODE_FUNNY_ACTION == fsm.get_mODE_FUNNY_ACTION());
}

void Lifecycle::run()
{
    fsmTimer.run(PERIOD_STRATEGY);
    readInputs();
    fsm.runCycle();
    publishOutputs();
}

void Lifecycle::registerMatchType(String const& name, IStrategy* install, IStrategy* match, IStrategy* funny)
{
    ASSERT_TEXT(name != "", "Strat name shall not be empty");
    ASSERT_TEXT(nbRegisteredStrats < NB_MAX_STRATEGIES, "Too many matchs registered.");
    matchs[nbRegisteredStrats].name     = name;
    matchs[nbRegisteredStrats].install  = install;
    matchs[nbRegisteredStrats].match    = match;
    matchs[nbRegisteredStrats].funny    = funny;
    matchs[nbRegisteredStrats].linear    = NULL;
    nbRegisteredStrats++;
}

void Lifecycle::registerLinearStrat(String const& name, StrategyFunctor functor)
{
    ASSERT_TEXT(name != "", "Strat name shall not be empty");
    ASSERT_TEXT(nbRegisteredStrats < NB_MAX_STRATEGIES, "Too many matchs registered.");
    ASSERT_TEXT(functor, "You registered a NULL functor");
    matchs[nbRegisteredStrats].name     = name;
    matchs[nbRegisteredStrats].install  = NULL;
    matchs[nbRegisteredStrats].match    = NULL;
    matchs[nbRegisteredStrats].funny    = NULL;
    matchs[nbRegisteredStrats].linear  = functor;
    nbRegisteredStrats++;
}

void Lifecycle::registerSelftest(IStrategy* _selftest)
{
    ASSERT_TEXT(selftest==NULL, "You already registered a selftest");
    ASSERT_TEXT(_selftest, "You registered a NULL selftest");
    selftest = _selftest;
}

void Lifecycle::networkConfigRequest(uint8_t strategyId_, eColor matchColor, bool _simulated)
{
    configureMatch(strategyId_, matchColor, _simulated);
    fsm.raise_networkConfigRequest();
    nav.simulated = _simulated;
}

void Lifecycle::configureColor()
{
    //Read color input
    eColor selectedColor = eColor_UNKNOWN;
    if ( hmi.isColorSwitchOnPrefered() )
        selectedColor = eColor_PREF;
    else
        selectedColor = eColor_SYM;

    //Read strat config
    configureMatch(hmi.getStrategyId(), selectedColor, false);
}

void Lifecycle::displayColor()
{
	eRgb selectedColor;

    if ( hmi.isColorSwitchOnPrefered() )
        selectedColor = YELLOW;
    else
        selectedColor = BLUE;

    hmi.ledRGB.set(selectedColor, FAST_BLINK);
}

void Lifecycle::networkStartRequest()
{
    fsm.raise_networkStartRequest();
}

void Lifecycle::beep(sc_integer nb)
{
    hmi.buzzer.bip(nb);
}

void Lifecycle::bootUp()
{
    if( listener )
        listener->bootUp();
    LOG_INFO(String("Robot is booted successfully, it took ") + millis() + " ms.");
}

void Lifecycle::ledRGB(sc_integer color, sc_integer blink)
{
    ASSERT(color < eRgb_Max);
    ASSERT(blink < eLedState_Max);
    hmi.ledRGB.set((eRgb)(color), eLedState(blink));
}

void Lifecycle::displayMatchRGB(sc_integer blink)
{
    ASSERT(blink < eLedState_Max);

    switch (matchColor) {
        case eColor_PREF:
            hmi.ledRGB.set(YELLOW, eLedState(blink));
            break;
        case  eColor_SYM:
            hmi.ledRGB.set(BLUE, eLedState(blink));
            break;

        default:
            ASSERT(false);
            break;
    }

}


void Lifecycle::readInputs()
{
    fsm.set_startIn                 (hmi.isStartPlugged());
    fsm.set_strategyRemainingTime   (chrono.getStrategyRemainingTime());
    fsm.set_matchDuration           (chrono.getTime());
}

void Lifecycle::publishOutputs()
{
    switch (currentMode)
    {
        case MODE_CORE_MATCH:
        {
            if(matchs[strategyId].match)
                matchs[strategyId].match->update(PERIOD_STRATEGY);

            if(matchs[strategyId].linear)
                matchs[strategyId].linear(*reinterpret_cast<Robot2017*>(listener) /* #porky */);
            break;
        }
        case MODE_FUNNY_ACTION:
        {
            if(matchs[strategyId].funny)
                matchs[strategyId].funny->update(PERIOD_STRATEGY);
            break;
        }
        case MODE_INSTALL:
        {
            if(matchs[strategyId].install)
                matchs[strategyId].install->update(PERIOD_STRATEGY);
            break;
        }
        case MODE_SELFTEST:
        {
            if(selftest)
                selftest->update(PERIOD_STRATEGY);
            break;
        }
        case MODE_NONE:
        {
            break;
        }
        
        default:
            ASSERT(false);
            break;
    }
}

void Lifecycle::displayStrategies()
{
    LOG_INFO("Available matchs : ");
    for (int i = 0; i < NB_MAX_STRATEGIES; ++i)
    {
        if( matchs[i].name != "" )
        {
            sleepMs(10);//Let Log thread do its job
            LOG_INFO("    [" + String(i) + "]: " + matchs[i].name);
        }
    }
}

bool Lifecycle::isSimulated()
{
    return simulated;
}

sc_integer Lifecycle::getModeStatus()
{
    switch (currentMode) {
        case MODE_CORE_MATCH:
            if(matchs[strategyId].match)
                return matchs[strategyId].match->getStatus();
            else
                return fsm.get_sUCCESS();
            break;
        case MODE_FUNNY_ACTION:
            if(matchs[strategyId].funny)
                return matchs[strategyId].funny->getStatus();
            else
                return fsm.get_sUCCESS();
            break;
        case MODE_INSTALL:
            if(matchs[strategyId].install)
                return matchs[strategyId].install->getStatus();
            else
                return fsm.get_sUCCESS();
            break;
        case MODE_SELFTEST:
            if(selftest)
                return selftest->getStatus();
            else
                return fsm.get_sUCCESS();
            break;
        case MODE_NONE:
        default:
            ASSERT(false);
            return fsm.get_nONE();
            break;
    }

}

void Lifecycle::startMode(sc_integer mode)
{
    ASSERT(currentMode == fsm.get_mODE_NONE());
    ASSERT(    mode == fsm.get_mODE_CORE_MATCH()
            || mode == fsm.get_mODE_FUNNY_ACTION()
            || mode == fsm.get_mODE_INSTALL()
            || mode == fsm.get_mODE_SELFTEST());
    currentMode = mode;

    LOG_DEBUG(String("[Lifecycle] mode started : ")+mode);

    switch (mode) {
        case MODE_CORE_MATCH:
            //start coutning match time
            chrono.startMatch();

            //starts the machine
            if( matchs[strategyId].match != NULL)
                matchs[strategyId].match->start();
            break;
        case MODE_FUNNY_ACTION:
            if(matchs[strategyId].funny)
                matchs[strategyId].funny->start();
            break;
        case MODE_INSTALL:
            if(matchs[strategyId].install)
                matchs[strategyId].install->start();
            break;
        case MODE_SELFTEST:
            if(selftest)
                selftest->start();
            break;
        case MODE_NONE:
        default:
            ASSERT(false);
            break;
    }
}

void Lifecycle::stopMode()
{
    ASSERT(currentMode != fsm.get_mODE_NONE());

    LOG_DEBUG(String("[Lifecycle] mode stopped : ")+currentMode);

    //dispatch match end
    if(currentMode == fsm.get_mODE_CORE_MATCH() && listener)
        listener->coreMatchEnded();

    if(currentMode == fsm.get_mODE_FUNNY_ACTION() && listener)
        listener->funnyActionEnded();

    currentMode = fsm.get_mODE_NONE();
}

void Lifecycle::configureMatch(uint8_t strategyId_, eColor _matchColor, bool _simulated)
{
    simulated = _simulated;
    detection.simulated = _simulated;

    //Check selected strategy
    if( nbRegisteredStrats <= strategyId_)
    {
        LOG_ERROR(String("Strategy ") +strategyId_+ " is unknown, using strat 0 instead.");
        strategyId_ = 0;
    }
    ASSERT_TEXT(matchs[strategyId].name != "", "Selected strategy is malformed.");
    strategyId = strategyId_;
    LOG_INFO(String("User has selected strategy [") + strategyId_ + "] " + matchs[strategyId].name + ".");

    //Set position
    if(matchs[strategyId].install)
    {
    	nav.setPosition(matchs[strategyId].install->getStartPosition());
    }

    //Configure color
    matchColor = _matchColor;
    if(listener)
        listener->colorChoosed(matchColor);

    //Simualtion
    if(simulated)
        LOG_INFO("<<< CAUTION : robot is SIMULATED >>>");
}
