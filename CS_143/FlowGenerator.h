#ifndef __CS_143__FlowGenerator__
#define __CS_143__FlowGenerator__

#include <iostream>
#include <tuple>
#include "EventGenerator.h"
#include "Flow.h"

class FlowGenerator : EventGenerator
{
public:
    // Constructor takes a vector of flows and add FlowEvents to the eventHeap
    // Flow format: (source, destination, size, start time)
    FlowGenerator(std::vector<Flow> flows, std::string flow_id);
    
    void giveEvent(std::unique_ptr<Event>);
};

#endif /* defined(__CS_143__FlowGenerator__) */
