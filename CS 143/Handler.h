#include "EventGenerator.h"
#include "Event.h"


class Handler {

private:
    // container for "current events"
    std::vector<Event*:> currEvents;
    // container for list of EventGenerators
    std::vector<EventGenerator*> generators;

public:
    // iterate over generators and find the "most immediate" time
    float getMinTime();
    // iterate over generators, populate currEvents with imm. events
    void populateCurrentEvents(float time);

    // hand off all events in currEvents to wherever they should go
    void processCurrentEvents();

    // find the DO ERRYTHANG
    void step();
}
