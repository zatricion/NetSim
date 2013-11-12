#ifndef __CS_143__PacketEvent__
#define __CS_143__PacketEvent__

#include "EventGenerator.h"
#include "Packet.h"
#include <string>

class PacketEvent : public Event
{
public:
    Packet& packet;
    PacketEvent(std::string dest, std::string src, float ts, Packet& pkt);
};

#endif /* defined(__CS_143__PacketEvent__) */
