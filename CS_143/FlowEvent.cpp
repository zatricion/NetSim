#include "FlowEvent.h"

// Constructor

FlowEvent::FlowEvent(Flow flowobj, int ds, std::string dest, std::string src, float ts) : Event(dest, src, ts)
{
    floww = flowobj;
    data_size = ds;
}

