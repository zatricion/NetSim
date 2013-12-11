#include "FlowGenerator.h"


/**
 * Creates a FlowGenerator object.  The constructor takes in a list of flows, 
 * which is uses to populate its event_heap with FlowEvents.  These events are
 * fired at the appropriate times by the Handler to start the data flows.
 *
 * @param flows the list of flows.
 * @param flow_id the id of the flow generator.
 */
FlowGenerator::FlowGenerator(std::vector<std::shared_ptr<Flow> > flows,
                             std::string flow_id) {
    for (auto it : flows) {
        auto flow_event = std::make_shared<FlowEvent>(it, it->destination, 
            it->source, it->timestamp);
        event_heap.push(flow_event);
    }
    uuid = flow_id;
}


/**
 * This method should never be called on the FlowGenerator, since it cannot
 * receive events.
 *
 * @param e the event given to the FlowGenerator.
 */
void FlowGenerator::giveEvent(std::shared_ptr<Event> e) {
    assert(false);
}
