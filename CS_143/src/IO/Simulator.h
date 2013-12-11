/*! \mainpage Network Simulator Documentation
 *
 * \section intro_sec Introduction
 *
 * Code developed by Group 4 for CS 143 (fall 2013) at Caltech.  Written and
 * developed by Max Horton, Alex Jose, Michael Lauria, Anish Agarwal, and 
 * Johno Ikpeazu.
 *
 * This is an event based simulation.  The Event objects, as well as the Handler
 * for Events, are found in src/EventHandling.  The network objects (Hosts,
 * Links, and Routers) are part of the EventGenerator class, and can be found
 * in src/EventGenerators.
 *
 * Implementation details can be found in the 'Classes' and 'Files' tabs.
 *
 * \section install_sec Installation
 *
 * \subsection pkg Required Software:
 * C++11, Boost version 1.48, bison, flex, gnuplot
 *
 * \subsection obt Obtaining the Code
 * Code can be found at: https://github.com/zatricion/NetSim.git
 *
 * \section running Usage
 *
 * \subsection requirements Input Requirements:
 * Invalid inputs can prevent the network simulation from functioning properly.
 * A few reasonable assumptions were made regarding input.  Specifically, DO NOT
 * create any flows with negative timestamps.  Also, please ensure that the
 * names of flows are globally unique.  If two flows have the same name, one may
 * intercept packets from the other.  Similarly, names of Hosts, Links, and
 * Routers must be globally unique.
 *
 * \subsection running Running the Code:
 * Sample configuration files for test cases 0, 1, and 2 can be found in
 * NetSim/test_cases.  They demonstrate how the flows, links, routers, and
 * hosts can be specified.  To run the code, use:
 *
 * ./simulator
 * Enter a time (in seconds) for the simulation to run: 20
 * Enter the name of the file that specifies the test case: test_cases/test_1_tahoe.txt
 */

/**
 * Simulator class.
 */
#ifndef __CS_143__Test__
#define __CS_143__Test__


#include <iostream>
#include <memory>
#include <cassert>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "../IO/Plotter.h"
#include "../IO/NetInput.h"
#include "../EventHandling/Handler.h"


int main();
void io();


#endif
