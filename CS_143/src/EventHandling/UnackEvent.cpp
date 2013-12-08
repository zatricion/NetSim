#include "UnackEvent.h"

// Constructor

UnackEvent::UnackEvent(std::shared_ptr<Packet> pkt,
		       std::string dest, std::string src, 
		       double ts) : Event(dest, src, ts) {
    packet = pkt;
}

std::string UnackEvent::toString() {
    std::stringstream fmt;
    fmt << "{UNACK EVENT: source=" << source << "destination=" <<
        destination << ", timestamp=" << timestamp << ", packet=" <<
        packet->toString() << "}.";
    return fmt.str();
}

std::string UnackEvent::getType() {
    return "UNACK_EVENT";
}
