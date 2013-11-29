#ifndef __CS_143__TCPRenoUpdateEvent__
#define __CS_143__TCPRenoUpdateEvent__

#include <string>
#include "Event.h"

class TCPRenoUpdateEvent : public Event
{
public:
    TCPRenoUpdateEvent(std::string dest, std::string src, float ts, int cavCount, std::string id);
//    Event(std::string dest, std::string src, float ts);
    int congAvCount;
    std::string flowID;
    
    virtual std::string printEvent();
    virtual std::string getType();
    
protected:
    float timestamp;
};

#endif /* defined(__CS_143__TCPRenoUpdateEvent__) */
