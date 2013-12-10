#ifndef __CS_143__Test__
#define __CS_143__Test__

#include <iostream>
#include <memory>
#include "../EventHandling/Packet.h"
#include "../EventGenerators/Flow.h"
#include "../EventGenerators/VegasFlow.h"
#include "../EventGenerators/TahoeFlow.h"
#include "../EventHandling/Event.h"
#include "../EventHandling/UnackEvent.h"
#include "../EventHandling/FlowEvent.h"
#include "../EventHandling/PacketEvent.h"
#include "../EventGenerators/EventGenerator.h"
#include "../EventGenerators/FlowGenerator.h"
#include "../EventGenerators/Router.h"
#include "../EventGenerators/Host.h"
#include "../EventGenerators/Link.h"
#include "../EventHandling/Handler.h"
#include <cassert>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "../Tools/Log.h"
#include "../IO/Plotter.h"
#include "../IO/NetInput.h"

int main();
void io();

#endif
