#include "Handler.h"


/**
 * Constructor for the event Handler.
 *
 * @param gen a map of all EventGenerators, with keys as strings
 */
void Handler::addGenerator(std::shared_ptr<EventGenerator> gen) {
    genMap[gen->getID()] = std::move(gen);
}


/**
 * Get the lowest time from the distributed event_heap.
 *
 * @return the minimum time.
 */
double Handler::getMinTime() {
    assert(genMap.size() > 0);

    double minTime = -1.0;
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


/**
 * Get a list of all events that should happen next.  This list will often
 * contain only 1 event, since it is unlikely that multiple Events will have
 * the exact same timestamp.
 *
 * @param minTime the minimum Event time out of all Events in all the
 * EventGenerator event_heaps
 */
void Handler::populateCurrentEvents(double minTime) {
    currEvents.clear();
    for (const auto& it : genMap) {
        // check to see if current EG has event at desired time, add if so
        if (it.second->hasEvents() && it.second->getNextTime() == minTime) {
            currEvents.push_back(it.second->getEvent());
        }
    }
}


/**
 * Handle all events in currEvents.
 */
void Handler::processCurrentEvents() {
    not_done = false;
    for (auto& it : currEvents) {
        not_done = true;
        handleEvent(std::move(it));
    }
}


/**
 * Handle a single Event by passing it to its destination, and allowing the 
 * destination object to do the rest of the work.
 */
void Handler::handleEvent(std::shared_ptr<Event> e) {
    genMap[e->destination]->giveEvent(e);
}


/**
 * Go through a single step of the simulation.
 */
void Handler::step() {
    populateCurrentEvents(getMinTime());
    processCurrentEvents();
}


/**
 * Tells if the Handler is still running.
 *
 * @return true if it is still running
 */
bool Handler::running() {
    return not_done;
}
