/**
 * A wrapper for the Packet class.  Used to pass Packets between
 * EventGenerators.
 */


#ifndef __CS_143__PacketEvent__
#define __CS_143__PacketEvent__


#include <string>
#include <sstream>
#include "Packet.h"
#include "../EventGenerators/EventGenerator.h"

class PacketEvent : public Event {

public:
    // Fields
    /** The Packet being sent. */
    std::shared_ptr<Packet> packet;

    // Constructor.
    PacketEvent(std::string dest, std::string src, double ts, std::shared_ptr<Packet> pkt);
    
    // Accessors.
    virtual std::string toString() override;
    virtual std::string getType() override;
};


#endif /* defined(__CS_143__PacketEvent__) */
