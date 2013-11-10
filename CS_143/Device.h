#ifndef __CS_143__Device__
#define __CS_143__Device__

#include "EventGenerator.h"

class Device : public EventGenerator
{
    void giveEvent(std::unique_ptr<Event>);
    std::unique_ptr<Event> getEvent();
    
    void sendPacket(Packet);
};


#endif /* defined(__CS_143__Device__) */
