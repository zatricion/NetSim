#include "UnackEvent.h"

// Constructor

UnackEvent::UnackEvent(Packet& pkt,
		       std::string dest, std::string src, 
		       float ts) : Event(dest, src, ts) {
    packet = pkt;
}

