#ifndef __CS_143__UnackEvent__
#define __CS_143__UnackEvent__

#include "EventGenerator.h"
#include "Event.h"
#include "Packet.h"
#include <string>

class UnackEvent : public Event {
public:
    UnackEvent(std::shared_ptr<Packet> pkt,
	       std::string dest, std::string src,
	       float ts);

    std::shared_ptr<Packet> packet;
    
    virtual void printEvent() override;
    virtual std::string getType() override;
};

#endif /* defined(__CS_143__UnackEvent__) */
