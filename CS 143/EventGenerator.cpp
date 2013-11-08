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

int EventGenerator::nextTimestamp()
{
    Event nextEvent = eventHeap.top();
    return nextEvent.timestamp;
}


// Device Methods
