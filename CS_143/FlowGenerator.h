#ifndef __CS_143__FlowGenerator__
#define __CS_143__FlowGenerator__

#include <iostream>
#include <tuple>
#include "EventGenerator.h"

class FlowGenerator : EventGenerator
{
public:
    // Constructor takes a vector of flows and add FlowEvents to the eventHeap
    // Flow format: (source, destination, size, start time)
    FlowGenerator::FlowGenerator(std::vector<std::tuple<std::string, std::string, int, float>> flows);
};

#endif /* defined(__CS_143__FlowGenerator__) */
