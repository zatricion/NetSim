#include "TCPRenoUpdateEvent.h"
#include <sstream>

// Constructor

TCPRenoUpdateEvent::TCPRenoUpdateEvent(std::string dest, std::string src, float ts) : Event(dest, src, ts) {
}

std::string TCPRenoUpdateEvent::printEvent()
{
    std::stringstream fmt;
    fmt << "{TCPRenoUpdateEVENT: source=" << source << "destination=" <<
        destination << "}.";
    return fmt.str();
}

std::string TCPRenoUpdateEvent::getType()
{
    return "TCPRenoUpdateEVENT";
}
