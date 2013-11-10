
#include "FlowEvent.h"

// Constructor

FlowEvent::FlowEvent(int data_size,
		     std::unique_ptr<EventGenerator> dest, 
		     std::unique_ptr<EventGenerator> src, 
		     float ts)
  : Event (dest, src, ts) {
    int data_size = data_size;
}

