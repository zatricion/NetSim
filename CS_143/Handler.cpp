#include "Handler.h"
#include <cassert>


// Take ref to an instantiated network object to be added to the simulation
// Handles getting that object's data
void Handler::addGenerator(std::unique_ptr<EventGenerator> gen) {
    genMap[gen->getID()] = gen;
}


float Handler::getMinTime() {
    assert(genMap.size() > 0);
    float minTime = -1;
    // iterate over EventGenerators
    for (auto it = genMap.begin(); it != genMap.end(); it++) {
        // update min if smaller
        float newTime = it->second->getNextTime();
        if (newTime < minTime || minTime < 0) // gross
	    minTime = newTime;
    }
    return minTime;
}


// iterate over generators, populate currEvents with imm. events
void Handler::populateCurrentEvents(float minTime) {
    // WARNING: will clear out currEvents...is that desired behavior?
    currEvents.clear();
    for (auto it = genMap.begin(); it != genMap.end(); it++) {
        // check to see if current EG has event at desired time, add if so
        if (it->second->getNextTime() == minTime) 
            currEvents.push_back(it->second->getEvent());
    }
}

// handle all events in current events queue
void Handler::processCurrentEvents() {
    for (auto it = currEvents.begin(); it != currEvents.end(); it++)
        handleEvent(*it);
}

// handle passed event by sending to its destination
void Handler::handleEvent(std::unique_ptr<Event> event) {
    genMap[event->destination]->giveEvent(event);
}

void Handler::step() {
    populateCurrentEvents(getMinTime());
    processCurrentEvents();
}
