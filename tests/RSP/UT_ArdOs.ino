/**
 * This file is the main file of the operationnal embedded robot code.
 */
#include "ArdOs.h"

using namespace ard;

Signal s1;
Signal s2;
Mutex m1;


/**
 Expected test trace :
 ---------------------
[ArdOs] Start scheduler.
[ArdOs] Task2 is running.
Task2 hello (shall appear first), giving hand to Task1
[ArdOs] Task1 is running.
Task1 hello, giving hand back to Task2
Task2 has received the priority again, locking the mutex will provoke a priority inversion
Task1 has received the priority due to priority inversion
Task2 has received the priority back after the priority inversion
Task2 is messing with a dual mutex lock, deadlocking itself infinitly, Task1 will run.
Task1 waiting on s2, it shall prevent it to exit

 */

//------------------------------------------------------------------------------
// high priority thread to set pin low
static void Task1() {
	Serial.println("Task1 hello, giving hand back to Task2");

	//This should not blocked as the signal has been set previsouly by Task2
	s2.wait();

	//take a mutex to check priority inversion
	m1.acquire();

	//Give hand back to Task2
	s1.set();

	//this section is only reachable if Task2 has taken the mutex, creating the priority inversion
	Serial.println("Task1 has received the priority due to priority inversion");
	m1.release();

	//s2 is never set again, so Task1 should never end
	Serial.println("Task1 waiting on s2, it shall prevent it to exit");
	s2.wait();
}

//------------------------------------------------------------------------------
// high priority thread to set pin low
static void Task2() {
	Serial.println("Task2 hello, giving hand to Task1");

	//Choke s2 to check that we can send a set() several time, the next wait shall empty them all
	s2.set();
	s2.set();
	s2.set();

	//check what happen when a mutex is released first
	m1.release();
	m1.release();
	m1.release();

	//Wait than Task1 give hand back
	s1.wait();

	Serial.println("Task2 has received the priority again, locking the mutex will provoke a priority inversion");

	m1.acquire();
	Serial.println("Task2 has received the priority back after the priority inversion");
	m1.release();

	//check that set/wait in a raw has no effect
	s1.set();
	s1.wait();

	//check the mess with 2 mutex locks
	m1.acquire();
	Serial.println("Task2 is messing with a dual mutex lock, deadlocking itself infinitly, Task1 will run.");
	m1.acquire();
}


//------------------------------------------------------------------------------
void setup() {
	g_ArdOs.init();
	s1.init();
	s2.init();
	m1.init();
	g_ArdOs.createThread_C("Task1", Task1, configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY + 1);
	g_ArdOs.createThread_C("Task2", Task2, configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY + 2);
	g_ArdOs.start();
}

//------------------------------------------------------------------------------
void loop() {
	// idle loop has a very small, configMINIMAL_STACK_SIZE, stack
	// loop must never block
	// the heartbeat movement is a proof that the CPU is not overused as the minimal priority task is executed
	g_ArdOs.kickHeartbeat();
}
