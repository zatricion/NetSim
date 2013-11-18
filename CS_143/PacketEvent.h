#ifndef __CS_143__PacketEvent__
#define __CS_143__PacketEvent__

#include "EventGenerator.h"
#include "Packet.h"
#include <string>

class PacketEvent : public Event
{
public:
	std::shared_ptr<Packet> packet;
    PacketEvent(std::string dest, std::string src, float ts, std::shared_ptr<Packet> pkt);
    
    virtual std::string printEvent() override;
    virtual std::string getType() override;
};

#endif /* defined(__CS_143__PacketEvent__) */
