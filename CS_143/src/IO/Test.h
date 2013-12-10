#ifndef __CS_143__Test__
#define __CS_143__Test__

#include <iostream>
#include <memory>
#include "../EventHandling/Packet.h"
#include "../EventGenerators/Flow.h"
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
#include "../EventGenerators/CongestionAlg.h"
#include "../EventGenerators/TCPReno.h"
#include "../EventGenerators/TCPVegas.h"
#include <cassert>
#include <cmath>
#include "../Tools/Log.h"
#include "../IO/Plotter.h"
#include "../IO/NetInput.h"

int main();
void packetTest();
void simTest0();
void simTest1();
void simTest2();
void simTest3();

#endif
