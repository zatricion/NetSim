#ifndef __CS_143__Test__
#define __CS_143__Test__

#include <iostream>
#include <memory>
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
#include "Log.h"

int main();
void packetTest();
void simTest0();

#endif
