#include "BFResendEvent.h"
#include <sstream>


/**
 * Creates a BFResendEvent.  This is a timed update event that causes a
 * Router to send its routing table.
 *
 * @param dest the Router to send its routing table.
 * @param src the Router that threw the update event (same as dest).
 * @param ts the time for the sending to occur.
 */
BFResendEvent::BFResendEvent(std::string dest, std::string src, double ts) :
                             Event(dest, src, ts) {
}


/**
 * Get the BFResendEvent as a string.
 *
 * @return a string representing the BFResendEvent.
 */
std::string BFResendEvent::toString() {
    std::stringstream fmt;
    fmt << "{BFResendEVENT: source=" << source << ", destination=" <<
        destination << ", timestamp=" << timestamp << "}.";
    return fmt.str();
}


/**
 * Get the type of the BFResendEvent, as a string.
 *
 * @return a string representing the type.
 */
std::string BFResendEvent::getType() {
    return "BF_RESEND_EVENT";
}
