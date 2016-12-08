@echo Uploading %2
@mode %1:1200,n,8,1
@timeout 1
@%USERPROFILE%\AppData\Local\Arduino15\packages\arduino\tools\bossac\1.6.1-arduino\bossac.exe -i -U false  --port=%1 -e -w -b %2 -R
