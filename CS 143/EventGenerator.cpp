//
//  CS 143
//
//  Event Generator
//  * Device
//   - Host
//   - Router
//  * Link

#include "Event.h" // TODO: Write event class
#include <queue>

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
