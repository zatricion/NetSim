#include "Device.h"

std::unique_ptr<Event> Device::getEvent()
{
    Event nextEvent = eventHeap.top();
    eventHeap.pop();
    return std::unique_ptr<Event>(&nextEvent);
}