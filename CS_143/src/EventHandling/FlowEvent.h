#ifndef __CS_143__FlowEvent__
#define __CS_143__FlowEvent__


class Host;

#include "../EventGenerators/EventGenerator.h"
#include "Event.h"
#include "Packet.h"
#include "../EventGenerators/Flow.h"
#include <string>

class FlowEvent : public Event {
public:
    FlowEvent(std::shared_ptr<Flow> flowobj,
              std::string dest,
              std::string src,
              double ts);

    int data_size;
    std::shared_ptr<Flow> floww;
    
    virtual std::string toString() override;
    virtual std::string getType() override;
};

#endif /* defined(__CS_143__FlowEvent__) */
