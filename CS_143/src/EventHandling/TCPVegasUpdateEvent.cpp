#include "TCPVegasUpdateEvent.h"
#include <sstream>

// Constructor

TCPVegasUpdateEvent::TCPVegasUpdateEvent(std::string dest, std::string src,
    float ts, std::string id) : Event(dest, src, ts) {
    flowID = id;
}

std::string TCPVegasUpdateEvent::toString() {
    std::stringstream fmt;
    fmt << "{TCPVegasUpdateEVENT: source=" << source << ", destination=" <<
        destination << ", flowID=" << flowID << ", timestamp=" << timestamp
        << "}.";
    return fmt.str();
}

std::string TCPVegasUpdateEvent::getType() {
    return "TCP_VEGAS_UPDATE_EVENT";
}
