/* 
* TestStrategies.h
*
* Created: 08/12/2016 00:17:42
* Author: wix
*/


#ifndef __TESTSTRATEGIES_H__
#define __TESTSTRATEGIES_H__

namespace ard
{
	//for debug only : light a led for each activated user switch
	void Strategy_ButtonTest();
	
	//for debug only : light a led for each activated omron
	void Strategy_OmronTest();
	
	//for debug only : make all leds blinking to check they are working
	void Strategy_LedTest();
}

#endif //__TESTSTRATEGIES_H__
