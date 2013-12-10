/**
 * Handler that runs the Event-based simulation.  Pulls Events from a list of
 * EventGenerators, and passes them to other EventGenerators.
 */


#ifndef __CS_143__Handler__
#define __CS_143__Handler__


#include <iostream>
#include <cassert>
#include <unordered_map>
#include <memory>
#include "Event.h"
#include "PacketEvent.h"
#include "../EventGenerators/EventGenerator.h"
#include "../Tools/Log.h"


class Handler {

private:
    // Fields
    /** Container for current events. */
    std::vector<std::shared_ptr<Event> > currEvents;

    /** Map from generator ID to EventGenerators. */
    std::unordered_map<std::string, std::shared_ptr<EventGenerator> > genMap;
    
    /** Is the simulation finished? */
    bool not_done = true;
    

public:
    // Constructor
    Handler() {};

    // Accessors
    bool running();

    // Methods
    double getMinTime();
    void populateCurrentEvents(double time);
    void processCurrentEvents();
    void step();
    void addGenerator(std::shared_ptr<EventGenerator> gen);
    void handleEvent(std::shared_ptr<Event> event);
};


#endif /* defined(__CS_143__Handler__) */
