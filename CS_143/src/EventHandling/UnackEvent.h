/**
 * An UnackEvent is called when an Event times out.  For example, if a data
 * Packet is sent, we queue an UnackEvent for the data Packet.  If the Packet
 * still hasn't been acked when the UnackEvent is called, it is resent.
 */


#ifndef __CS_143__UnackEvent__
#define __CS_143__UnackEvent__


#include <string>
#include <sstream>
#include "Event.h"
#include "Packet.h"
#include "../EventGenerators/EventGenerator.h"


class UnackEvent : public Event {

public:
    // Fields
    /** The packet that may not have been acked. */
    std::shared_ptr<Packet> packet;

    // Constructors
    UnackEvent(std::shared_ptr<Packet> pkt,
	       std::string dest, std::string src,
	       double ts);

    // Accessors
    virtual std::string toString() override;
    virtual std::string getType() override;
};


#endif /* defined(__CS_143__UnackEvent__) */
