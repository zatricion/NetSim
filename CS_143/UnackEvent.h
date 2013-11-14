#ifndef __CS_143__UnackEvent__
#define __CS_143__UnackEvent__

#include "EventGenerator.h"
#include "Event.h"
#include "Packet.h"
#include <string>

class UnackEvent : public Event {
public:
    UnackEvent(Packet& pkt,
	       std::string dest, std::string src,
	       float ts);

    Packet packet;
    
    virtual void printEvent() override;
};

#endif /* defined(__CS_143__UnackEvent__) */
