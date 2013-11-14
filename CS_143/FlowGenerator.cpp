#include "FlowGenerator.h"
#include "FlowEvent.h"

FlowGenerator::FlowGenerator(std::vector<std::shared_ptr<Flow> > flows, std::string flow_id)
{
    // Go through flows, create FlowEvents, push onto eventHeap
    for (std::vector<std::shared_ptr<Flow> >::iterator it = flows.begin();
         it != flows.end();
         it++)
    {
        // create FlowEvent
        FlowEvent flow_event = FlowEvent((*it)->getptr(), (*it)->destination, (*it)->source, (*it)->timestamp);
        
        // add event to eventHeap
        eventHeap.push(std::make_shared<FlowEvent>(flow_event));
    }
    
    uuid = flow_id;
}

