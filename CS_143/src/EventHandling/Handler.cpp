#include "Handler.h"
#include "PacketEvent.h"
#include <iostream>
#include <cassert>


// Take ref to an instantiated network object to be added to the simulation
// Handles getting that object's data
void Handler::addGenerator(std::shared_ptr<EventGenerator> gen) {
    genMap[gen->getID()] = std::move(gen);
}


float Handler::getMinTime() {
    FILE_LOG(logDEBUG) << "Getting MinTime.";
    assert(genMap.size() > 0);


    float minTime = -1.0;
    // iterate over EventGenerators
    //for (auto it = genMap.begin(); it != genMap.end(); it++) {
    for (auto& it : genMap) {

        // Make sure it has events.  Otherwise move on.
        if (!it.second->hasEvents()) { continue; }
        // update min if smaller
        float newTime = it.second->getNextTime();
        if (newTime < minTime || minTime < 0.0) {// gross
	    minTime = newTime;
        }
    }
    return minTime;
}


// iterate over generators, populate currEvents with imm. events
void Handler::populateCurrentEvents(float minTime) {
    FILE_LOG(logDEBUG) << "Populating current events.";
    // WARNING: will clear out currEvents...is that desired behavior?
    currEvents.clear();
    for (auto it = genMap.begin(); it != genMap.end(); it++) {
        // check to see if current EG has event at desired time, add if so
        if (it->second->hasEvents() && it->second->getNextTime() == minTime) {
            currEvents.push_back(it->second->getEvent());
        }
    }
}

// handle all events in current events queue
void Handler::processCurrentEvents() {
    FILE_LOG(logDEBUG) << "Processing current events.";
    not_done = false;
    //assert(false);
    for (auto it = currEvents.begin(); it != currEvents.end(); it++) {
    //for (auto it : currEvents) {
        not_done = true;
        handleEvent(std::move(*it));
    }
}

// handle passed event by sending to its destination
void Handler::handleEvent(std::shared_ptr<Event> e) {
    FILE_LOG(logDEBUG) << "Handling a single event.";
    e->printEvent();
    genMap[e->destination]->giveEvent(e);
}

void Handler::step() {
    FILE_LOG(logDEBUG) << "Running another step of simulation.";
    populateCurrentEvents(getMinTime());
    processCurrentEvents();
}


bool Handler::running()
{
    return not_done;
}
