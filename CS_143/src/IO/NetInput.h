#ifndef __NETINPUT_H_GUARD__
#define __NETINPUT_H_GUARD__

#include "../EventHandling/Handler.h"


// function headers
//void read_input(Handler &h);
Handler& read_input();
//void parse(std::istream &datafile, Handler &handler);
Handler& parse(std::istream &datafile);

#endif // __NETINPUT_H_GUARD__
