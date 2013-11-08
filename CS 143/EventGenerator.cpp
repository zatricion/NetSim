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
#include <queue>

void EventGenerator::getId()
{
    return id;
}

void EventGenerator::addEvent(Event* e)
{
    eventHeap.push(e);
}

void EventGenerator::getNextTime()
{
    return eventHeap.top()->getTime();
}

Event* EventGenerator::getEvent() {
    return eventHeap.pop();
}
