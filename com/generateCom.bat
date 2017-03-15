REM you may uncomment this to desactivate proto generation, it may speed up the build
REM exit 0

echo ------------- generating com --------------------------
@echo off


REM This script generates C and python source code from protocol buffer specifications
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
set PROTOC=%ARD_PATH%\com\nanopb-0.3.7-windows-x86\generator-bin\protoc.exe
set NANOPB_INC=%ARD_PATH%\com\nanopb-0.3.7-windows-x86\generator\proto
set MSG_PATH=%ARD_PATH%\com\msg
set PYTHON_OUT_PATH=%ARD_PATH%\com\generated
set C_OUT_PATH=%ARD_PATH%\com\generated

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
	call %PROTOC% --nanopb_out=%C_OUT_PATH% --python_out=%PYTHON_OUT_PATH% -I %NANOPB_INC% --proto_path=%MSG_PATH% %MSG_PATH%\%%i
)

echo
echo -------------------------------------------------------

cd %ARD_PATH%/com
python setup.py build_ext --inplace

echo -------------------------------------------------------