#include "UnackEvent.h"


/**
 * Create an UnackEvent.
 *
 * @param pkt the Packet that may not have been acked.
 * @param dest the destination of the UnackEvent.  For example, for a data
 * Packet, this should be the Host that attempted to send the Packet.
 * @param src the source of the UnackEvent.  Should be same as destination.
 * @param ts the time at which the UnackEvent is fired.
 */
UnackEvent::UnackEvent(std::shared_ptr<Packet> pkt, std::string dest,
                       std::string src, double ts) : 
                       Event(dest, src, ts) {
    packet = pkt;
}


/**
 * Get a string representing the UnackEvent.
 *
 * @return a string representing the UnackEvent.
 */
std::string UnackEvent::toString() {
    std::stringstream fmt;
    fmt << "{UNACK EVENT: source=" << source << "destination=" <<
        destination << ", timestamp=" << timestamp << ", packet=" <<
        packet->toString() << "}.";
    return fmt.str();
}


/**
 * Get a string representing the type of an UnackEvent.
 *
 * @return the string representing the type of the UnackEvent.
 */
std::string UnackEvent::getType() {
    return "UNACK_EVENT";
}
