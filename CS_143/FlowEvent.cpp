#include "FlowEvent.h"
#include <sstream>

// Constructor

FlowEvent::FlowEvent(std::shared_ptr<Flow> flowobj, std::string dest, std::string src, float ts) : Event(src, src, ts)
{
    floww = std::move(flowobj);
}

std::string FlowEvent::printEvent()
{
    std::stringstream fmt;
    fmt << "{PKT EVENT: source=" << source << "destination=" <<
        destination << "}.";
    return fmt.str();
}

std::string FlowEvent::getType()
{
    return "FLOW_EVENT";
}
