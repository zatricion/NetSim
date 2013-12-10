#ifndef __NETINPUT_H_GUARD__
#define __NETINPUT_H_GUARD__

#include "../EventHandling/Handler.h"
#include <iostream>
#include <fstream>

// function headers
Handler& read_input(std::ifstream);
Handler& parse(std::ifstream &datafile);

#endif // __NETINPUT_H_GUARD__
