#include "FlowGenerator.h"

FlowGenerator::FlowGenerator(std::vector<std::tuple<std::string, std::string, int, float>> flows)
{
    // Go through flows, create FlowEvents, push onto eventHeap
    for (std::vector<std::tuple>::iterator it = flows.begin();
         it != flows.end();
         it++)
    {
        // unpack flow
        std::tie (source, dest, size, time) = *it;
        
        // create FlowEvent
        flow = FlowEvent(source, dest, size, time);
        
        // add event to eventHeap
        eventHeap.push(flow);
    }
}
