#include "Event.h"
#include <sstream>

// Constructor

Event::Event(std::string dest,
             std::string src,
             double ts) {
    destination = std::move(dest);
    source = std::move(src);
    timestamp = ts;
}

double Event::eventTime() const {
    return timestamp;
}


std::string Event::toString()
{
    std::stringstream fmt;
    fmt << "{EVENT: source=" << source << "destination=" <<
        destination << "}.";
    return fmt.str();
}

std::string Event::getType()
{
    return "EVENT";
}
