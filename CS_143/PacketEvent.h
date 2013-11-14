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
    
    virtual void printEvent() override;
    virtual std::string getType() override;
};

#endif /* defined(__CS_143__PacketEvent__) */
