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

std::shared_ptr<Event> EventGenerator::getEvent()
{
    std::shared_ptr<Event> nextEvent = std::move(eventHeap.top());
    eventHeap.pop();
    return nextEvent;
}

bool EventGenerator::hasEvents() {
    return eventHeap.size() != 0;
}

// Add event to local priority queue.
void EventGenerator::addEventToLocalQueue(std::shared_ptr<Event> e)
{
    eventHeap.push(e);
}
