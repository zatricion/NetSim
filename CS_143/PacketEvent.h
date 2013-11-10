#include "EventGenerator.h"
#include "Packet.h"
#include <string>

class PacketEvent : Event
{
public:
    PacketEvent(Packet &pkt, 
		std::unique_ptr<EventGenerator> dest, 
		std::unique_ptr<EventGenerator> src, 
		float ts);

    Packet &packet;
};

