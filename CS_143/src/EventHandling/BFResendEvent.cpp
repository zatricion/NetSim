#include "BFResendEvent.h"
#include <sstream>

// Constructor

BFResendEvent::BFResendEvent(std::string dest, std::string src,
    double ts) : Event(dest, src, ts) {
}

std::string BFResendEvent::toString() {
    std::stringstream fmt;
    fmt << "{BFResendEVENT: source=" << source << ", destination=" <<
        destination << ", timestamp=" << timestamp << "}.";
    return fmt.str();
}

std::string BFResendEvent::getType() {
    return "BF_RESEND_EVENT";
}
