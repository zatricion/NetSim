#include "TCPRenoUpdateEvent.h"
#include <sstream>

// Constructor

TCPRenoUpdateEvent::TCPRenoUpdateEvent(std::string dest, std::string src,
    float ts, int cavCount, std::string id) : Event(dest, src, ts) {

    congAvCount = cavCount;
    flowID = id;
}

std::string TCPRenoUpdateEvent::printEvent()
{
    std::stringstream fmt;
    fmt << "{TCPRenoUpdateEVENT: source=" << source << "destination=" <<
        destination << ", start=" << congAvCount << "}.";
    return fmt.str();
}

std::string TCPRenoUpdateEvent::getType()
{
    return "TCP_RENO_UPDATE_EVENT";
}
