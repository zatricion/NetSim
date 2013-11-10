#include "EventGenerator.h"
#include "Packet.h"
#include <string>

class FlowEvent : public Event {
public:
    FlowEvent(std::unique_ptr<EventGenerator> dest, 
	      std::unique_ptr<EventGenerator> src, 
	      float ts);

    int data_size = data_size;
};

