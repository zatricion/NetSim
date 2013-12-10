#include "TCPVegasUpdateEvent.h"


/**
 * Create an update event for TCPVegas.  This will induce VegasFlow to call
 * its handleVegasUpdate() method.
 */
TCPVegasUpdateEvent::TCPVegasUpdateEvent(std::string dest, std::string src,
    double ts, std::string id) : Event(dest, src, ts) {
    flowID = id;
}


/**
 * Get a string representing the Event.
 *
 * @return a string representing the Event.
 */
std::string TCPVegasUpdateEvent::toString() {
    std::stringstream fmt;
    fmt << "{TCPVegasUpdateEVENT: source=" << source << ", destination=" <<
        destination << ", flowID=" << flowID << ", timestamp=" << timestamp
        << "}.";
    return fmt.str();
}


/**
 * Get a string representing the VegasUpdateEvent type.
 *
 * @return a string representing the type.
 */
std::string TCPVegasUpdateEvent::getType() {
    return "TCP_VEGAS_UPDATE_EVENT";
}
