#ifndef __CS_143__FlowEvent__
#define __CS_143__FlowEvent__

#include "EventGenerator.h"
#include "Event.h"
#include "Packet.h"
#include <string>

class FlowEvent : public Event {
public:
    FlowEvent(int ds,
              std::string dest,
              std::string src,
              float ts);

    int data_size;
};

#endif /* defined(__CS_143__FlowEvent__) */