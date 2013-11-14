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


void Event::printEvent()
{
    fprintf(stdout, "\nEVENT\n");
    fprintf(stdout, "------------------------\n");
    fprintf(stdout, "Source: %s\n", source.c_str());
    fprintf(stdout, "Destination: %s\n\n", destination.c_str());
}

std::string Event::getType()
{
    return "EVENT";
}
