@echo off
echo ------------- generating com --------------------------

REM This script generates C and python source code from protocol buffer specifications
REM %1 : path to the ard2017 git folder
REM %2 : verbosity configuration (on/off)

if [%1] == [] (
	set ARD_PATH=.
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
set PROTOC=%ARD_PATH%\devenv\nanopb-0.3.7-windows-x86\generator-bin\protoc.exe
set MSG_PATH=%ARD_PATH%\msg
set PYTHON_OUT_PATH=%ARD_PATH%\vizu\proto
set C_OUT_PATH=%ARD_PATH%\robot\Common\2_AL\proto

REM inform user about configuration
echo Running from folder       : %ARD_PATH% 
echo Using protoc              : %PROTOC%
echo Reading messages          : %MSG_PATH%
echo Generating C     files in : %C_OUT_PATH%
echo Generating pyhon files in : %PYTHON_OUT_PATH% 

echo ------------------------------------------------------- 

REM List all files in the MSG_PATH folder and execute protoc on them for both C and python
for /F %%i in ('dir /B/D %MSG_PATH%\*.proto') do (
	echo Processing %%i
	
	REM If the file is Types.proto, a special treatment is operated as the generation goes elsewhere
	if [%%i] == [Types.proto] (
		call %PROTOC% --nanopb_out=%ARD_PATH%\robot\Common\1_RSP --python_out=%PYTHON_OUT_PATH% --proto_path=%MSG_PATH% %MSG_PATH%\%%i
	) else (
		call %PROTOC% --nanopb_out=%C_OUT_PATH% --python_out=%PYTHON_OUT_PATH% --proto_path=%MSG_PATH% %MSG_PATH%\%%i
	)
	
	
)

echo
echo -------------------------------------------------------