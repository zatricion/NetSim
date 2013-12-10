#include "FlowEvent.h"


/**
 * Constructor for a FlowEvent.
 *
 * @param flowobj the Flow held by the FlowEvent.
 * @param dest the Host that is the sending end of the Flow.
 * @param src the FlowGenerator holding the FlowEvent.
 * @param ts the time at which the Flow will be passed to the Host and start
 * sending SYN packets.
 */
FlowEvent::FlowEvent(std::shared_ptr<Flow> flowobj, std::string dest,
                     std::string src, double ts) : Event(src, src, ts) {
    flow = std::move(flowobj);
}


/**
 * Get a string representing the FlowEvent.
 *
 * @return a string representing the FlowEvent.
 */
std::string FlowEvent::toString() {
    std::stringstream fmt;
    fmt << "{FLOW_EVENT: source=" << source << ", destination=" <<
        destination << ", timestamp=" << timestamp << "}.";
    return fmt.str();
}


/**
 * Get the type of the FlowEvent.
 *
 * @return a string representing the FlowEvent type
 */
std::string FlowEvent::getType()
{
    return "FLOW_EVENT";
}
