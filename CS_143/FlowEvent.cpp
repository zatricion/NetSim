#include "FlowEvent.h"

// Constructor

FlowEvent::FlowEvent(std::shared_ptr<Flow> flowobj, std::string dest, std::string src, float ts) : Event(src, src, ts)
{
    floww = std::move(flowobj);
}

void FlowEvent::printEvent()
{
    fprintf(stdout, "\nFLOW EVENT\n");
    fprintf(stdout, "------------------------\n");
    fprintf(stdout, "Source: %s\n", source.c_str());
    fprintf(stdout, "Destination: %s\n\n", destination.c_str());
}

std::string FlowEvent::getType()
{
    return "FLOW_EVENT";
}