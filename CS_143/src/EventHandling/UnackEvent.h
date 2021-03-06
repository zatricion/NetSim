#ifndef __CS_143__UnackEvent__
#define __CS_143__UnackEvent__

#include "../EventGenerators/EventGenerator.h"
#include "Event.h"
#include "Packet.h"
#include <string>
#include <sstream>

class UnackEvent : public Event {
public:
    UnackEvent(std::shared_ptr<Packet> pkt,
	       std::string dest, std::string src,
	       float ts);

    std::shared_ptr<Packet> packet;
    
    virtual std::string printEvent() override;
    virtual std::string getType() override;
};

#endif /* defined(__CS_143__UnackEvent__) */
