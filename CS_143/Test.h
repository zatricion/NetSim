#ifndef __CS_143__Test__
#define __CS_143__Test__

#include <iostream>
#include "Packet.h"
#include "Flow.h"
#include "Event.h"
#include "UnackEvent.h"
#include "FlowEvent.h"
#include "PacketEvent.h"
#include "EventGenerator.h"
#include "FlowGenerator.h"
#include "Router.h"
#include "Host.h"
#include "Link.h"
#include "Handler.h"
#include "CongestionAlg.h"
#include <cassert>
#include <cmath>

int main();
void packetTest();
void eventTest();
void simTest0();

#endif