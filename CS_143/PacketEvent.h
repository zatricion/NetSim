#ifndef __CS_143__PacketEvent__
#define __CS_143__PacketEvent__

#include "EventGenerator.h"
#include "Packet.h"
#include <string>

class PacketEvent : public Event
{
public:
    PacketEvent(Packet& pkt,
                std::unique_ptr<EventGenerator> dest,
                std::unique_ptr<EventGenerator> src,
                float ts);

    Packet& packet;
};

#endif /* defined(__CS_143__PacketEvent__) */
