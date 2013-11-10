#include "FlowGenerator.h"
#include "FlowEvent.h"

FlowGenerator::FlowGenerator(std::vector<std::tuple<std::string, std::string, int, float> > flows)
{
    // Go through flows, create FlowEvents, push onto eventHeap
    for (std::vector<std::tuple<std::string, std::string, int, float>>::iterator it = flows.begin();
         it != flows.end();
         it++)
    {
        // unpack flow
        std::string source;
        std::string dest;
        int size;
        float time;
        std::tie (source, dest, size, time) = *it;
        
        // create FlowEvent
        FlowEvent flow = FlowEvent(source, dest, size, time);
        
        // add event to eventHeap
        eventHeap.push(flow);
    }
}
