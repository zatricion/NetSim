#ifndef __CS_143__TimeoutEvent__
#define __CS_143__TimeoutEvent__

#include <string>
#include "Event.h"

class TimeoutEvent: public Event
{
public:
    TimeoutEvent(std::string dest, std::string src, double ts, int start, std::string);
//    Event(std::string dest, std::string src, double ts);
    int fastRecoveryCount;
    std::string flowID;
    
    virtual std::string toString() override;
    virtual std::string getType() override;
};

#endif /* defined(__CS_143__TimeoutEvent__) */
