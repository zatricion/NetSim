#include "Event.h"

// Constructor

Event::Event(std::string dest,
             std::string src,
             float ts) {
    destination = std::move(dest);
    source = std::move(src);
    timestamp = ts;
}

float Event::eventTime() const {
    return timestamp;
}

// Override operator to allow eventHeap to order by timestamp
bool Event::operator>(const Event& other) const {
    return timestamp > other.timestamp;
}


void Event::printEvent()
{
    fprintf(stdout, "\nEVENT\n");
    fprintf(stdout, "------------------------\n");
    fprintf(stdout, "Source: %s\n", source.c_str());
    fprintf(stdout, "Destination: %s\n\n", destination.c_str());
}