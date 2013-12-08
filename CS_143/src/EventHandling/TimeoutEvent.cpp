#include "TimeoutEvent.h"
#include <sstream>

// Constructor

TimeoutEvent::TimeoutEvent(std::string dest, std::string src, double ts, int frCount, std::string id) : Event(dest, src, ts) {
    fastRecoveryCount = frCount;
    flowID = id;
}

std::string TimeoutEvent::toString()
{
    std::stringstream fmt;
    fmt << "{TimeoutEvent: source=" << source << ", destination=" <<
        destination << ", start=" << fastRecoveryCount << ", timestamp=" << timestamp << "}.";
    return fmt.str();
}

std::string TimeoutEvent::getType()
{
    return "TIMEOUT_EVENT";
}
