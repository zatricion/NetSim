#include "FlowGenerator.h"
#include "../EventHandling/FlowEvent.h"

/**
 * Creates a FlowGenerator object.  The constructor takes in a list of flows, 
 * which is uses to populate its eventHeap with FlowEvents.  These events are
 * fired at the appropriate times by the Handler to start the data flows.
 *
 * @param flows the list of flows.
 * @param flow_id the id of the flow generator.
 */
FlowGenerator::FlowGenerator(std::vector<std::shared_ptr<Flow> > flows,
                             std::string flow_id) {
    // Go through flows, create FlowEvents, push onto eventHeap
    //for (std::vector<std::shared_ptr<Flow> >::iterator it = flows.begin();
         //it != flows.end();
         //it++)
    for (auto it : flows) {
        auto flow_event = std::make_shared<FlowEvent>(it, it->destination, 
            it->source, it->timestamp);
        eventHeap.push(flow_event);
    }
    uuid = flow_id;
}

