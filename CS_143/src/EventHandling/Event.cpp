#include "Event.h"


/**
 * Constructor for a generic Event.
 * 
 * @param dest the destination of the Event (where the Event should land).
 * @param src the creator of the Event.
 * @param ts the time at which the event is fired
 */
Event::Event(std::string dest,
             std::string src,
             double ts) {
    destination = std::move(dest);
    source = std::move(src);
    timestamp = ts;
}


/**
 * Get the time of the Event.
 */
double Event::eventTime() const {
    return timestamp;
}


/**
 * Get a string representing the Event.
 */
std::string Event::toString() {
    std::stringstream fmt;
    fmt << "{EVENT: source=" << source << "destination=" <<
        destination << "}.";
    return fmt.str();
}


/**
 * Get the type of the Event, as a string.
 */
std::string Event::getType() {
    return "EVENT";
}
