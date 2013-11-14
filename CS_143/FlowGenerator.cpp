#include "FlowGenerator.h"
#include "FlowEvent.h"

FlowGenerator::FlowGenerator(std::vector<Flow> flows, std::string flow_id)
{
    // Go through flows, create FlowEvents, push onto eventHeap
    for (std::vector<Flow>::iterator it = flows.begin();
         it != flows.end();
         it++)
    {
        // create FlowEvent
        FlowEvent flow_event = FlowEvent(make_unique<Flow>(*it), it->destination, it->source, it->timestamp);
        
        // add event to eventHeap
        eventHeap.push(&flow_event);
    }
    
    uuid = flow_id;
}

void FlowGenerator::giveEvent(std::unique_ptr<Event>) {};