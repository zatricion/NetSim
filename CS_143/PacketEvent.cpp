#include "PacketEvent.h"

// Constructor

PacketEvent::PacketEvent(std::string dest, std::string src, float ts, Packet& pkt) : Event(dest, src, ts), packet(pkt) {
    packet = pkt;
}
