As the arduino devenv is very simple, it's not possible 
to have unit tests reusing operationnal code.

The method is :
- copy the UT*.ino test you want to execute to the robots folder
- rename robots.ino to robots.ino.bak
- rename UT*.ino to robots.ino
- execute the test
- delete rebots.ino
- rename robots.ino.bak to robots.ino

Yeah, it's a pain in the ass.