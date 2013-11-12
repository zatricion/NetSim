#include "FlowEvent.h"

// Constructor

FlowEvent::FlowEvent(std::unique_ptr<Flow> flowobj, std::string dest, std::string src, float ts) : Event(dest, src, ts)
{
    floww = std::move(flowobj);
}

