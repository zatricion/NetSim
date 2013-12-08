#include "FlowEvent.h"
#include <sstream>

// Constructor

FlowEvent::FlowEvent(std::shared_ptr<Flow> flowobj, std::string dest, std::string src, double ts) : Event(src, src, ts)
{
    floww = std::move(flowobj);
}

std::string FlowEvent::toString()
{
    std::stringstream fmt;
    fmt << "{FLOW_EVENT: source=" << source << ", destination=" <<
        destination << ", timestamp=" << timestamp << "}.";
    return fmt.str();
}

std::string FlowEvent::getType()
{
    return "FLOW_EVENT";
}
