echo "8==>"
set PROTOC="C:\Users\wix\Documents\GitHub\ard2017\devenv\nanopb-0.3.7-windows-x86\generator-bin\protoc.exe"
call %PROTOC% --nanopb_out=%1 --proto_path=%1 %1\Teleop.proto
pause