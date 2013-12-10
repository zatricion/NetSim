/** 
 * A FlowEvent contains a Flow, and lives in the FlowGenerator's eventHeap
 * until the Flow should start.  Then, the FlowEvent passes the flow to the
 * correct Host.
 */


#ifndef __CS_143__FlowEvent__
#define __CS_143__FlowEvent__


class Host;


#include "../EventGenerators/EventGenerator.h"
#include "Event.h"
#include "Packet.h"
#include "../EventGenerators/Flow.h"
#include <string>


class FlowEvent : public Event {
public:
    // Fields
    /** The data size of the Flow. */
    int data_size;
    /** The flow. */
    std::shared_ptr<Flow> flow;

    // Constructor
    FlowEvent(std::shared_ptr<Flow> flowobj,
              std::string dest,
              std::string src,
              double ts);

    // Accessors
    virtual std::string toString() override;
    virtual std::string getType() override;
};


#endif /* defined(__CS_143__FlowEvent__) */
