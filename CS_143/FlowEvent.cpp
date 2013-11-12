#include "FlowEvent.h"

// Constructor

FlowEvent::FlowEvent(std::string flowId, int ds, std::string dest, std::string src, float ts) : Event(dest, src, ts)
{
    id = flowId;
    data_size = ds;
}

