#include "FlowEvent.h"

// Constructor

FlowEvent::FlowEvent(int ds,
                     std::unique_ptr<EventGenerator> dest,
                     std::unique_ptr<EventGenerator> src,
                     float ts)
{
    destination = std::move(dest);
    source = std::move(src);
    timestamp = ts;
    data_size = ds;
}

