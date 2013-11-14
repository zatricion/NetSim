#ifndef __CS_143__FlowEvent__
#define __CS_143__FlowEvent__


class Host;

#include "EventGenerator.h"
#include "Event.h"
#include "Packet.h"
#include "Flow.h"
#include <string>

class FlowEvent : public Event {
public:
    FlowEvent(std::shared_ptr<Flow> flowobj,
              std::string dest,
              std::string src,
              float ts);

    int data_size;
    std::shared_ptr<Flow> floww;
    
    virtual void printEvent() override;
    virtual std::string getType() override;
};

#endif /* defined(__CS_143__FlowEvent__) */
