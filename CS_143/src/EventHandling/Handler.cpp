#include "Handler.h"


/**
 * Constructor for the event Handler.
 *
 * @param gen a map of all EventGenerators, with keys as strings
 */
void Handler::addGenerator(std::shared_ptr<EventGenerator> gen) {
    gen_map[gen->getID()] = std::move(gen);
}


/**
 * Get the lowest time from the distributed event_heap.
 *
 * @return the minimum time.
 */
double Handler::getMinTime() {
    assert(gen_map.size() > 0);

    double min_time = -1.0;
    for (auto& it : gen_map) {
        // Make sure it has events.  Otherwise move on.
        if (!it.second->hasEvents()) { continue; }
        // update min if smaller
        double new_time = it.second->getNextTime();
        if (new_time < min_time || min_time < 0.0) {
	    min_time = new_time;
        }
    }
    return min_time;
}


/**
 * Get a list of all events that should happen next.  This list will often
 * contain only 1 event, since it is unlikely that multiple Events will have
 * the exact same timestamp.
 *
 * @param min_time the minimum Event time out of all Events in all the
 * EventGenerator event_heaps
 */
void Handler::populateCurrentEvents(double min_time) {
    curr_events.clear();
    for (const auto& it : gen_map) {
        // check to see if current EG has event at desired time, add if so
        if (it.second->hasEvents() && it.second->getNextTime() == min_time) {
            curr_events.push_back(it.second->getEvent());
        }
    }
}


/**
 * Handle all events in curr_events.
 */
void Handler::processCurrentEvents() {
    not_done = false;
    for (auto& it : curr_events) {
        not_done = true;
        handleEvent(std::move(it));
    }
}


/**
 * Handle a single Event by passing it to its destination, and allowing the 
 * destination object to do the rest of the work.
 */
void Handler::handleEvent(std::shared_ptr<Event> e) {
    gen_map[e->destination]->giveEvent(e);
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
