#include "FlowEvent.h"

// Constructor

FlowEvent::FlowEvent(int ds,
                     std::unique_ptr<EventGenerator> dest,
                     std::unique_ptr<EventGenerator> src,
                     float ts) : Event (dest, src, ts) {
    data_size = ds;
}

