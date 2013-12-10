/**
 * An object used to store Flow objects until the data flow is ready to begin.
 * It holds an eventHeap of FlowEvents that are passed to the Handler when the
 * data flow should begin.  The Handler processes these FlowEvents, and creates
 * a data flow on the proper Host.
 */


#ifndef __CS_143__FlowGenerator__
#define __CS_143__FlowGenerator__


#include <iostream>
#include <tuple>
#include <cassert>
#include "EventGenerator.h"
#include "Flow.h"
#include "../EventHandling/FlowEvent.h"


class FlowGenerator : public EventGenerator {

public:
    // Constructor
    FlowGenerator(std::vector<std::shared_ptr<Flow> > flows, std::string flow_id);
    
    // Methods
    void giveEvent(std::shared_ptr<Event> e);
};


#endif /* defined(__CS_143__FlowGenerator__) */
