#ifndef __CS_143__Test__
#define __CS_143__Test__

#include <iostream>
#include <memory>
#include "Packet.h"
#include "EventGenerators/Flow.h"
#include "Event.h"
#include "UnackEvent.h"
#include "FlowEvent.h"
#include "PacketEvent.h"
#include "EventGenerators/EventGenerator.h"
#include "EventGenerators/FlowGenerator.h"
#include "EventGenerators/Router.h"
#include "EventGenerators/Host.h"
#include "EventGenerators/Link.h"
#include "Handler.h"
#include "EventGenerators/CongestionAlg.h"
#include <cassert>
#include <cmath>
#include "Log.h"

int main();
void packetTest();
void simTest0();

#endif
