#ifndef __CS_143__BFResendEvent__
#define __CS_143__BFResendEvent__

#include <string>
#include "Event.h"

class BFResendEvent : public Event
{
public:
    BFResendEvent(std::string dest, std::string src, double ts);
//    Event(std::string dest, std::string src, double ts);
    
    virtual std::string toString() override;
    virtual std::string getType() override;
};

#endif /* defined(__CS_143__BFResendEvent__) */
