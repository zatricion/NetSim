#include "PacketEvent.h"

// Constructor

PacketEvent::PacketEvent(Packet &pkt, 
	     std::unique_ptr<EventGenerator> dest, 
	     std::unique_ptr<EventGenerator> src, 
	     float ts) 
    : Event (dest, src, ts){
    packet = pkt;
}
