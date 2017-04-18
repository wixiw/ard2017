@echo off

REM you may uncomment this to desactivate proto generation, it may speed up the build
REM exit 0

echo ------------- generating FSM --------------------------



REM This script generates C++ code from yakindu state machines
REM %1 : path to the ard2017 git folder
REM %2 : verbosity configuration (on/off)

if [%1] == [] (
	set ARD_PATH=..
) else (
	set ARD_PATH=%1
)

if [%2] == [] (
	set VERBOSE=off
) else (
	set VERBOSE=%2
)

@echo %VERBOSE%

REM Configure script
set INPUT=%ARD_PATH%\robot\Common\2_AL\

REM inform user about configuration
echo Running from folder       : %ARD_PATH% 
echo Using input from          : %INPUT%

echo -------------------------------------------------------

scc -d %INPUT% 

echo -------------------------------------------------------