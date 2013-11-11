#include "UnackEvent.h"

// Constructor

UnackEvent::UnackEvent(std::string packID, 
		       std::string dest, std::string src, 
		       float ts) : Event(dest, src, ts) {
    packetID = packID;
}

