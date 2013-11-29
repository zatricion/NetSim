#include "TCPRenoUpdateEvent.h"
#include <sstream>

// Constructor

TCPRenoUpdateEvent::TCPRenoUpdateEvent(std::string dest, std::string src, float ts, int start) : Event(dest, src, ts) {
    windowStart = start;
}

std::string TCPRenoUpdateEvent::printEvent()
{
    std::stringstream fmt;
    fmt << "{TCPRenoUpdateEVENT: source=" << source << "destination=" <<
        destination << ", start=" << windowStart << "}.";
    return fmt.str();
}

std::string TCPRenoUpdateEvent::getType()
{
    return "TCPRenoUpdateEVENT";
}
