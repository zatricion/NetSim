#include "EventGenerator.h"


/**
 * Get the ID of the EventGenerator.
 *
 * @return the ID as a string
 */
std::string EventGenerator::getID() const {
    return uuid;
}


/**
 * Get the lowest time in the EventGenerator's priority queue.
 *
 * @return the min time as a double
 */
double EventGenerator::getNextTime() {
    assert(hasEvents());
    return event_heap.top()->eventTime();
}


/**
 * Remove and return the lowest time event from the EventGenerator's priority
 * queue.
 *
 * @return a pointer to the removed Event.
 */
std::shared_ptr<Event> EventGenerator::getEvent() {
    std::shared_ptr<Event> next_event = std::move(event_heap.top());
    event_heap.pop();
    return next_event;
}


/**
 * Tells whether the EventGenerator's priority queue has any events.
 *
 * @return bool is true if there are events
 */
bool EventGenerator::hasEvents() {
    return !event_heap.empty();
}


/**
 * Add Event to local event_heap.
 *
 * @param e the Event to add
 */
void EventGenerator::addEventToLocalQueue(std::shared_ptr<Event> e) {
    event_heap.push(e);
}
