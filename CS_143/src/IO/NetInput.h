#ifndef __NETINPUT_H_GUARD__
#define __NETINPUT_H_GUARD__

#include "../EventHandling/Handler.h"


// function headers
Handler& read_input();
Handler& parse(std::istream &datafile);

#endif // __NETINPUT_H_GUARD__
