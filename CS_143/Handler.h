#ifndef __CS_143__Handler__
#define __CS_143__Handler__

#include "EventGenerator.h"
#include "Event.h"


class Handler {

private:
    // container for "current events"
    std::vector<std::unique_ptr<Event> > currEvents;
    
    // container for list of EventGenerators
    //std::vector<std::unique_ptr<EventGenerator> > generators;

    // Map from generator ID to unique_ptr<EventGenerator>
    unordered_map<std::string, std::unique_ptr<EventGenerator> > genMap;

public:
    // No-argument constructor
    Handler::Handler();

    // iterate over generators and find the "most immediate" time
    float getMinTime();
    
    // iterate over generators, populate currEvents with imm. events
    void populateCurrentEvents(float time);

    // hand off all events in currEvents to wherever they should go
    void processCurrentEvents();

    void step();

    // add an instantiated network object to the simulation
    void addGenerator(std::unique_ptr<EventGenerator> gen);

    // handle an event by passing it to where it should go
    void handleEvent(std::unique_ptr<Event> event);
};

#endif /* defined(__CS_143__Handler__) */
