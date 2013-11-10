
#include "Event.h"

// Constructor

Event::Event(std::unique_ptr<EventGenerator> dest, 
	     std::unique_ptr<EventGenerator> src, 
	     float ts) {
    destination = dest;
    source = src;
    timestamp = ts;
}

float Event::eventTime() const {
    return timestamp;
}

// Override operator to allow eventHeap to order by timestamp
bool Event::operator>(const Event& other) const {
    return timestamp > other.timestamp;
}

<<<<<<< HEAD
void Event::handleEvent() {
    std::unique_ptr<Event> self(*this);
    dest->giveEvent(self);
}
=======
void Event::handleEvent()
{
    // TODO: handleEvent for info events
    //       Also create other classes that inherit from Event (AckEvent, BfEvent, etc.)
}
>>>>>>> c5379ad44647621a95ca3ce51f24499e5f716d8e
