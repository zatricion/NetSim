//
//  CS 143
//
//  Event Generator
//  * Link
//  * Device
//   - Host
//   - Router

#include "EventGenerator.h"
#include "../Tools/Log.h"
#include <cassert>
#include <iostream>

/**
 * Get the ID of the Event Generator.
 *
 * @return the ID as a string
 */
std::string EventGenerator::getID() const {
    return uuid;
}

/**
 * Get the lowest time in the Event Generator's priority queue.
 *
 * @return the min time as a float
 */
float EventGenerator::getNextTime() {
    assert(hasEvents());
    return eventHeap.top()->eventTime();
}

/**
 * Remove and return the lowest time event from the Event Generator's priority
 * queue.
 *
 * @return a pointer to the Event.
 */
std::shared_ptr<Event> EventGenerator::getEvent() {
    std::shared_ptr<Event> nextEvent = std::move(eventHeap.top());
    eventHeap.pop();
    return nextEvent;
}

/**
 * Tells whether the Event Generator's priority queue has any events.
 *
 * @return bool is true if there are events
 */
bool EventGenerator::hasEvents() {
    return eventHeap.size() != 0;
}


/**
 * Add Event to local priority queue.
 *
 * @param E the Event to add.
 */
void EventGenerator::addEventToLocalQueue(std::shared_ptr<Event> e) {
    eventHeap.push(e);
}


/**
 * Get the size of the Event Generator's priority queue.
 *
 * @return a long representing the size
 */
long EventGenerator::heapSize() {
    return eventHeap.size();
}


/**
 * Returns a string representing the Event Generator.
 *
 * @return the string representing the Event Generator
 */
std::string EventGenerator::toString() {
    return "TODO";
}
