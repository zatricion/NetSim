#ifndef __CS_143__FlowEvent__
#define __CS_143__FlowEvent__

#include "EventGenerator.h"
#include "Event.h"
#include "Packet.h"
#include "Flow.h"
#include <string>

class FlowEvent : public Event {
public:
    FlowEvent(Flow flowobj,
              int ds,
              std::string dest,
              std::string src,
              float ts);

    int data_size;
    Flow floww;
};

#endif /* defined(__CS_143__FlowEvent__) */
