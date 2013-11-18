#include "UnackEvent.h"

// Constructor

UnackEvent::UnackEvent(std::shared_ptr<Packet> pkt,
		       std::string dest, std::string src, 
		       float ts) : Event(dest, src, ts) {
    packet = pkt;
}

std::string UnackEvent::printEvent()
{
    std::stringstream fmt;
    fmt << "{UNACK EVENT: source=" << source << "destination=" <<
        destination << "}.";
    return fmt.str();

}

std::string UnackEvent::getType()
{
    return "UNACK_EVENT";
}
