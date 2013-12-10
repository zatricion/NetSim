/**
 * A BFResendEvent will be used to Routers to resend their information for
 * Bellman-Ford.
 */


#ifndef __CS_143__BFResendEvent__
#define __CS_143__BFResendEvent__


#include <string>
#include <sstream>
#include "Event.h"


class BFResendEvent : public Event {

public:
    // Constructor
    BFResendEvent(std::string dest, std::string src, double ts);
    
    // Accessors
    virtual std::string toString() override;
    virtual std::string getType() override;
};


#endif /* defined(__CS_143__BFResendEvent__) */
