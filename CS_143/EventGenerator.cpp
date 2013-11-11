//
//  CS 143
//
//  Event Generator
//  * Link
//  * Device
//   - Host
//   - Router

#include "EventGenerator.h"

std::string EventGenerator::getId() const
{
    return uuid;
}

float EventGenerator::getNextTime()
{
    return eventHeap.top().eventTime();
}

