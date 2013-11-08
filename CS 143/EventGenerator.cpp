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

std::string EventGenerator::getId() const
{
    return uuid;
}

float EventGenerator::getNextTime()
{
    return eventHeap.top().eventTime();
}

