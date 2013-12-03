#ifndef __NETINPUT_H_GUARD__
#define __NETINPUT_H_GUARD__

#include "../EventHandling/Handler.h"


// function headers
void read_input(Handler &h);
void parse(std::istream &datafile, Handler &h);

#endif // __NETINPUT_H_GUARD__
