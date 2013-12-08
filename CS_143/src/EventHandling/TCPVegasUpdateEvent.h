#ifndef __CS_143__TCPVegasUpdateEvent__
#define __CS_143__TCPVegasUpdateEvent__

#include <string>
#include "Event.h"

class TCPVegasUpdateEvent : public Event
{
public:
    TCPVegasUpdateEvent(std::string dest, std::string src, double ts, std::string id);
//    Event(std::string dest, std::string src, double ts);
    std::string flowID;
    
    virtual std::string toString() override;
    virtual std::string getType() override;
};

#endif /* defined(__CS_143__TCPVegasUpdateEvent__) */
