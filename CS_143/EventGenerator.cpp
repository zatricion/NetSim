//
//  CS 143
//
//  Event Generator
//  * Link
//  * Device
//   - Host
//   - Router

#include "EventGenerator.h"
#include <cassert>

std::string EventGenerator::getID() const
{
    return uuid;
}

float EventGenerator::getNextTime()
{
    assert(hasEvents());
    return eventHeap.top()->eventTime();
}

std::unique_ptr<Event> EventGenerator::getEvent()
{
    Event nextEvent = *eventHeap.top();
    eventHeap.pop();
    return make_unique<Event>(nextEvent);
}

bool EventGenerator::hasEvents() {
    return eventHeap.size() != 0;
}

// Add event to local priority queue.
void EventGenerator::addEventToLocalQueue(Event* e)
{
    eventHeap.push(e);
}