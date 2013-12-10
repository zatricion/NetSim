#include "PacketEvent.h"
#include <sstream>


/**
 * Constructor.
 *
 * @param dest the destination of the Event (i.e. the Link, Host, or Router
 * that the Packet is being sent to)
 * @param src the source of the Event (previous EventGenerator that held Packet)
 * @param ts the time at which the Packet is passed
 * @param pkt the Packet to be passed
 */
PacketEvent::PacketEvent(std::string dest, std::string src, double ts, std::shared_ptr<Packet> pkt) : Event(dest, src, ts) {
    packet = pkt;
}


/**
 * Get the PacketEvent as a string.
 *
 * @return a string representing the PacketEvent.
 */
std::string PacketEvent::toString() {
    std::stringstream fmt;
    fmt << "{PKT EVENT: source=" << source << ", destination=" <<
        destination << ", timestamp=" << timestamp << ", packet=" << 
        packet->toString() << "}.";
    return fmt.str();
    
}


/**
 * Get a string representing the type of the PacketEvent.
 *
 * @return a string representing the type
 */
std::string PacketEvent::getType() {
    return "PACKET_EVENT";
}
