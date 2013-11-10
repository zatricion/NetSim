
#include "Event.h"

// Constructor

Event::Event(Packet &pkt, std::string dest, std::string src, float ts)
{
    packet = pkt;
    destination = dest;
    source = src;
    timestamp = ts;
}

float Event::eventTime() const
{
    return timestamp;
}

// Override operator to allow eventHeap to order by timestamp
bool Event::operator>(const Event& other) const
{
    return timestamp > other.timestamp;
}

void Event::handleEvent()
{
    // TODO: handleEvent for info events
    //       Also create other classes that inherit from Event (AckEvent, BfEvent, etc.)
}