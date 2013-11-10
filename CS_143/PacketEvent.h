
#include "Packet.h"
#include <string>

class Event
{
public:
    PacketEvent(Packet &pkt, 
		std::unique_ptr<EventGenerator> dest, 
		std::unique_ptr<EventGenerator> src, 
		float ts);

    Packet &packet;
};

