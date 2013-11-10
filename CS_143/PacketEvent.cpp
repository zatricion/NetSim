#include "PacketEvent.h"

// Constructor

PacketEvent::PacketEvent(Packet& pkt,
	     std::unique_ptr<EventGenerator> dest, 
	     std::unique_ptr<EventGenerator> src, 
                         float ts) : packet(pkt)
{
    destination = std::move(dest);
    source = std::move(src);
    timestamp = ts;
    packet = pkt;
}
