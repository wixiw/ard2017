#include "ComInterfaces.h"

using namespace ard;

void ISerialDriver::print(char const * text)
{
    while(*text)
    {
        write(*text);
        text++;
    }
}

void ISerialDriver::println(char const * text)
{
    print(text);
    write('\r');
    write('\n');
}

//required to have a proper linkage with some options ... weird ...
IComListener::~IComListener()
{
}

//required to have a proper linkage with some options ... weird ...
ICom::~ICom()
{
}

