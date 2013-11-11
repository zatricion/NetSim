#ifndef __CS_143__Handler__
#define __CS_143__Handler__

#include "EventGenerator.h"
#include "Event.h"


class Handler {

private:
    // container for "current events"
    std::vector<std::unique_ptr<Event> > currEvents;
    
    // container for list of EventGenerators
    std::vector<std::unique_ptr<EventGenerator> > generators;

    // Map from generator ID to unique_ptr<EventGenerator>
    unordered_map<std::string, unique_ptr<EventGenerator> > generators;

public:
    // No-argument constructor
    Handler::Handler();

    // iterate over generators and find the "most immediate" time
    float getMinTime();
    
    // iterate over generators, populate currEvents with imm. events
    void populateCurrentEvents(float time);

    // hand off all events in currEvents to wherever they should go
    void processCurrentEvents();

    // find the DO ERRYTHANG
    void step();

    void add_generator(string generator_id, unique_ptr<EventGenerator> generator);
};

#endif /* defined(__CS_143__Handler__) */
