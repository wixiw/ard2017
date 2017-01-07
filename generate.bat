@echo off
set PROTOC="C:\Users\wix\Documents\GitHub\ard2017\devenv\nanopb-0.3.7-windows-x86\generator-bin\protoc.exe"
call %PROTOC% --nanopb_out=%1robot\Common\2_AL\proto --python_out=%1vizu\proto --proto_path=%1msg %1msg\Teleop.proto