#include "Handler.h"
#include "PacketEvent.h"
#include <iostream>
#include <cassert>


// Take ref to an instantiated network object to be added to the simulation
// Handles getting that object's data
void Handler::addGenerator(std::shared_ptr<EventGenerator> gen) {
    genMap[gen->getID()] = std::move(gen);
}


double Handler::getMinTime() {
    assert(genMap.size() > 0);


    double minTime = -1.0;
    // iterate over EventGenerators
    //for (auto it = genMap.begin(); it != genMap.end(); it++) {
    for (auto& it : genMap) {

        // Make sure it has events.  Otherwise move on.
        if (!it.second->hasEvents()) { continue; }
        // update min if smaller
        double newTime = it.second->getNextTime();
        if (newTime < minTime || minTime < 0.0) {
	    minTime = newTime;
        }
    }
    return minTime;
}


// iterate over generators, populate currEvents with imm. events
void Handler::populateCurrentEvents(double minTime) {
    // WARNING: will clear out currEvents...is that desired behavior?
    currEvents.clear();
    for (const auto& it : genMap) {
        // check to see if current EG has event at desired time, add if so
        if (it.second->hasEvents() && it.second->getNextTime() == minTime) {
            currEvents.push_back(it.second->getEvent());
        }
    }
}

// handle all events in current events queue
void Handler::processCurrentEvents() {
    not_done = false;
    for (auto& it : currEvents) {
        not_done = true;
        handleEvent(std::move(it));
    }
}

// handle passed event by sending to its destination
void Handler::handleEvent(std::shared_ptr<Event> e) {
    // TODO remove
    // verbosely print the routing tables.
    for (const auto& it : genMap) {
        if (it.second->getID() == "RTABLES") {
            FILE_LOG(logDEBUG) << it.second->toString();
        }
    }
    FILE_LOG(logDEBUG) << e->toString();
    genMap[e->destination]->giveEvent(e);
}

void Handler::step() {
    populateCurrentEvents(getMinTime());
    processCurrentEvents();
}


bool Handler::running()
{
    return not_done;
}
