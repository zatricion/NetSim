//
//  CS 143
//
//  Event Generator
//  * Link
//  * Device
//   - Host
//   - Router

#include "EventGenerator.h"
#include "Event.h"
#include <cassert>
#include "Event.h" // TODO: Write event class
#include <queue>

int EventGenerator::nextTimestamp()
{
    Event nextEvent = eventHeap.top();
    return nextEvent.timestamp;
}


// Device Methods
void EventGenerator::getId() {
    return id;
}

void EventGenerator::addEvent(Event* e) {
    eventHeap.push(e);
}

void EventGenerator::getTime() {
    return eventHeap.top()->getTime();
}

Event* EventGenerator::getEvent() {
    return eventHeap.pop();
}
