/**
 * A mechanism by which VegasFlow algorithms are called to update their
 * window sizes.  See VegasFlow's handleVegasUpdate.
 */


#ifndef __CS_143__TCPVegasUpdateEvent__
#define __CS_143__TCPVegasUpdateEvent__


#include <string>
#include "Event.h"


class TCPVegasUpdateEvent : public Event {
public:
    // Fields
    /** The unique ID of the Flow. */
    std::string flowID;

    // Constructor
    TCPVegasUpdateEvent(std::string dest, std::string src, double ts, std::string id);
    
    // Accessors
    virtual std::string toString() override;
    virtual std::string getType() override;
};


#endif /* defined(__CS_143__TCPVegasUpdateEvent__) */
