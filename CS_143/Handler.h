#ifndef __CS_143__Handler__
#define __CS_143__Handler__

#include "EventGenerator.h"
#include "Event.h"
#include <unordered_map>
#include <memory>


class Handler {

private:
    // container for "current events"
    std::vector<std::shared_ptr<Event> > currEvents;

    // Map from generator ID to shared_ptr<EventGenerator>
    std::unordered_map<std::string, std::shared_ptr<EventGenerator> > genMap;
    
    bool not_done = true;
    

public:
    // No-argument constructor
    Handler() {};

    // iterate over generators and find the "most immediate" time
    float getMinTime();
    
    // iterate over generators, populate currEvents with imm. events
    void populateCurrentEvents(float time);

    // hand off all events in currEvents to wherever they should go
    void processCurrentEvents();

    void step();

    // add an instantiated network object to the simulation
    void addGenerator(std::shared_ptr<EventGenerator> gen);

    // handle an event by passing it to where it should go
    void handleEvent(std::shared_ptr<Event> event);
    
    // is currEvents empty?
    bool running();
};

#endif /* defined(__CS_143__Handler__) */
