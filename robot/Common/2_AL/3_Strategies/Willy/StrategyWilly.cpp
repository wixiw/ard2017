#include "3_Strategies/Strategies.h"
#include "Robot2017.h"
#include "Willy.h"

using namespace ard;

YakardTimer fsmTimer;
static Willy stratClass(fsmTimer);

void ard::Strategy_Willy (Robot2017& robot)
{
	LOG_INFO("Strategy_Willy.");
    
	//BIG FAT WARNING : je fais un init après le start c'est très moche
	stratClass.init();

	while(1)
	{
        fsmTimer.run(100);
	    stratClass.update(100);
	    ArdOs::sleepMs(100);
	}

	LOG_INFO("STRAT : Finished.");

	robot.dieMotherFucker();
}
