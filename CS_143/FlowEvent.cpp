#include "FlowEvent.h"

// Constructor

FlowEvent::FlowEvent(int ds, std::string dest, std::string src, float ts) : Event(dest, src, ts)
{
    data_size = ds;
}

