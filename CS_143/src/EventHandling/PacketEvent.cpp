#include "PacketEvent.h"
#include <sstream>

// Constructor

PacketEvent::PacketEvent(std::string dest, std::string src, float ts, std::shared_ptr<Packet> pkt) : Event(dest, src, ts) {
    packet = pkt;
}

std::string PacketEvent::printEvent()
{
    std::stringstream fmt;
    fmt << "{PKT EVENT: source=" << source << "destination=" <<
        destination << "}.";
    return fmt.str();
    
}

std::string PacketEvent::getType()
{
    return "PACKET_EVENT";
}
