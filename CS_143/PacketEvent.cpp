#include "PacketEvent.h"

// Constructor

PacketEvent::PacketEvent(Packet& pkt, 
			 std::string dest, std::string src, float ts) 
    : packet(pkt), Event(dest, src, ts) {
    packet = pkt;
}
