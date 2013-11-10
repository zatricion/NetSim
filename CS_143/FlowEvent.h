#include "EventGenerator.h"
#include "Event.h"
#include "Packet.h"
#include <string>

class FlowEvent : public Event {
public:
    FlowEvent(int ds,
              std::unique_ptr<EventGenerator> dest,
              std::unique_ptr<EventGenerator> src,
              float ts);

    int data_size;
};

